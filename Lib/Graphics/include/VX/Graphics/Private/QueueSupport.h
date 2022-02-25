#pragma once

#include <cstddef>
#include <optional>
#include <set>
#include <unordered_map>

#include <VX/Copyable.h>
#include <VX/Noncopyable.h>

namespace VX::Graphics::Private {

    enum class QueueFeature: size_t {
        Graphics, Presentation
    };

    class QueueSupport final {
        VX_DEFAULT_MOVABLE(QueueSupport);
        VX_MAKE_NONCOPYABLE(QueueSupport);
    private:
        std::unordered_map<QueueFeature, uint32_t> m_indexes;
    public:
        explicit QueueSupport(std::unordered_map<QueueFeature, uint32_t> indexes)
            : m_indexes(std::move(indexes))
        {}

        ~QueueSupport() = default;

        [[nodiscard]] bool has_feature(QueueFeature feature) const;
        [[nodiscard]] std::optional<uint32_t> queue_for_feature(QueueFeature feature) const;
        [[nodiscard]] std::set<uint32_t> unique_queues() const;

        template <QueueFeature TFeature>
        [[nodiscard]] uint32_t get_queue() const {
            return queue_for_feature(TFeature).value();
        }
    };

    class QueueSupportBuilder final {
        std::set<QueueFeature> m_required_features;
        std::unordered_map<QueueFeature, uint32_t> m_indexes;
    public:
        explicit QueueSupportBuilder(std::set<QueueFeature> required_features)
                : m_required_features(std::move(required_features))
        {}

        ~QueueSupportBuilder() = default;

        void add_feature_for_index(uint32_t index, QueueFeature feature);
        [[nodiscard]] bool is_fully_satisfied() const;
        [[nodiscard]] QueueSupport build() const;
    };
}
