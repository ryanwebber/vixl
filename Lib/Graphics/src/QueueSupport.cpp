#include <VX/Graphics/Private/QueueSupport.h>

namespace VX::Graphics::Private {

    bool QueueSupport::has_feature(QueueFeature feature) const
    {
        return m_indexes.contains(feature);
    }

    std::optional<uint32_t> QueueSupport::queue_for_feature(QueueFeature feature) const
    {
        if (!has_feature(feature))
            return { };

        return m_indexes.at(feature);
    }

    std::set<uint32_t> QueueSupport::unique_queues() const
    {
        std::set<uint32_t> indexes;
        for (const auto& [_, index] : m_indexes) {
            indexes.insert(index);
        }

        return indexes;
    }

    void QueueSupportBuilder::add_feature_for_index(uint32_t index, QueueFeature feature)
    {
        if (!m_required_features.contains(feature))
            return;

        if (m_indexes.contains(feature))
            return;

        m_indexes[feature] = index;
    }

    bool QueueSupportBuilder::is_fully_satisfied() const
    {
        return m_required_features.size() == m_indexes.size();
    }

    QueueSupport QueueSupportBuilder::build() const
    {
        return QueueSupport(m_indexes);
    }
}
