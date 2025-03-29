#include "autoanalysissettingsinteractor.h"
#include "domain/interfaces/business/icomparator.h"
#include "imageprocessorsmanager.h"


QList<shared_ptr<IComparator>> AutoAnalysisSettingsInteractor::getComparators() const {
    auto comparators = ImageProcessorsManager::instance()->getAllComparators();
    QList<std::shared_ptr<IComparator> > allowedComparators;

    std::copy_if (comparators.begin(),
                 comparators.end(),
                 std::back_inserter(allowedComparators),
                 [](std::shared_ptr<IImageProcessor> procesor)
    {
        auto comparator = dynamic_pointer_cast<IComparator>(procesor);
        return comparator != nullptr && comparator->isPartOfAutoReportingToolbox();
    });
    return allowedComparators;
}

void AutoAnalysisSettingsInteractor::saveComparatorState(const QString& shortName, bool isEnabled) {
    ImageProcessorsManager::instance()->setEnabledInAutoanalysisToolbox(shortName, isEnabled);
}
