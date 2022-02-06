#pragma once

#include <set>
#include <unordered_map>

namespace VX::Graphics::Private {

    enum class QueueFeature: size_t {
        Graphics, Presentation
    };

    class QueueBuilder {
        std::set<QueueFeature> m_required_features;
        std::unordered_map<QueueFeature, uint32_t> m_indexes;
    public:
        explicit QueueBuilder(std::set<QueueFeature> required_features)
            : m_required_features(std::move(required_features))
        {}

        ~QueueBuilder() = default;

        [[nodiscard]] bool is_fully_satisfied() const {
            return m_required_features.size() == m_indexes.size();
        }

        [[nodiscard]] bool has_feature(QueueFeature feature) const {
            return m_indexes.contains(feature);
        }

        [[nodiscard]] uint32_t queue_for_feature(QueueFeature feature) const {
            if (!has_feature(feature))
                throw std::runtime_error("No queue supports feature");

            return m_indexes.at(feature);
        }

        [[nodiscard]] std::set<uint32_t> unique_queues() const {
            std::set<uint32_t> indexes;
            for (const auto& [_, index] : m_indexes) {
                indexes.insert(index);
            }

            return indexes;
        }

        void add_feature_for_index(uint32_t index, QueueFeature feature) {
            if (!m_required_features.contains(feature))
                return;

            if (m_indexes.contains(feature))
                return;

            m_indexes[feature] = index;
        }
    };
}
