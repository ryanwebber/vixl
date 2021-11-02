use anyhow::*;
use rayon::prelude::*;
use glob::glob;
use std::fs::{read_to_string, write, create_dir_all};
use std::path::PathBuf;
use std::path::Path;
use std::env;

struct ShaderData {
    src: String,
    src_path: PathBuf,
    spv_path: PathBuf,
    kind: shaderc::ShaderKind,
}

impl ShaderData {
    pub fn load(src_path: PathBuf, out_dir: &Path) -> Result<Self> {

        let extension = src_path
            .extension()
            .and_then(|s| s.to_str())
            .context(format!("Unable to determine shader file extension: {}", src_path.to_str().unwrap()))?;

        let kind = match extension {
            "vert" => shaderc::ShaderKind::Vertex,
            "frag" => shaderc::ShaderKind::Fragment,
            "comp" => shaderc::ShaderKind::Compute,
            _ => bail!("Unsupported shader: {}", src_path.display()),
        };

        let spv_path = {
            let rel_path = src_path.strip_prefix("src/shaders").unwrap();
            let ext = format!("{}.spv", extension);

            out_dir
                .join(rel_path)
                .with_extension(ext)
        };

        let src = read_to_string(src_path.clone())?;

        Ok(Self {
            src,
            src_path,
            spv_path,
            kind,
        })
    }
}

fn main() -> Result<()> {

    let compiled_shader_dir = env::var("OUT_DIR")
        .map(|s| PathBuf::from(s))
        .map(|p| p.join("compiled-shaders"))
        .expect("Unable to determine build dir for compiled shaders.");

    let shader_paths = {
        let mut paths = Vec::new();
        paths.extend(glob("./src/**/*.vert")?);
        paths.extend(glob("./src/**/*.frag")?);
        paths.extend(glob("./src/**/*.comp")?);
        paths
    };

    let shaders = shader_paths.into_par_iter()
        .map(|glob_result| {
            ShaderData::load(glob_result?, &compiled_shader_dir)
        })
        .collect::<Vec<Result<_>>>()
        .into_iter()
        .collect::<Result<Vec<_>>>();

    let mut compiler = shaderc::Compiler::new().context("Unable to create shader compiler")?;

    // This can't be parallelized. The [shaderc::Compiler] is not
    // thread safe. Also, it creates a lot of resources. You could
    // spawn multiple processes to handle this, but it would probably
    // be better just to only compile shaders that have been changed
    // recently.
    for shader in shaders? {

        // Tells cargo to rerun this script if something in /src/ changes.
        println!("cargo:rerun-if-changed={}", shader.src_path.as_os_str().to_str().unwrap());

        let compiled = compiler.compile_into_spirv(
            &shader.src,
            shader.kind,
            &shader.src_path.to_str().unwrap(),
            "main",
            None,
        )?;

        create_dir_all(shader.spv_path.parent().unwrap())?;
        write(shader.spv_path, compiled.as_binary_u8())?;

        let out_dir_str = compiled_shader_dir
            .as_os_str()
            .to_str()
            .unwrap();

        // Tells cargo to set this env variable when compiling the source code. It's
        // used in the macro that includes the compiled shader into the final binary
        println!("cargo:rustc-env={}={}", "VIXL_COMPILED_SHADER_DIR", out_dir_str);
    }

    Ok(())
}
