#include "imageprocessorsmanagertests.h"

#include <tests/mocks/MockImageProcessor.h>

void ImageProcessorsManagerTests::initTestCase() {
    manager = ImageProcessorsManager::instance();
}

void ImageProcessorsManagerTests::cleanupTestCase() {
    // Reset manager state if necessary
}

void ImageProcessorsManagerTests::testFindProcessor() {
    // Add a mock processor
    auto processor = new MockImageProcessor("MockProcessor", "Ctrl+M", "A mock image processor", ImageProcessorType::Comparator);
    manager->addProcessor(processor);

    // Verify that the processor can be found by name
    AImageProcessor *found = manager->findProcessor("MockProcessor");
    QVERIFY(found != nullptr);
    QCOMPARE(found->name(), QString("MockProcessor"));

    // Verify that searching for a non-existent processor returns nullptr
    AImageProcessor *notFound = manager->findProcessor("NonExistentProcessor");
    QVERIFY(notFound == nullptr);
}

void ImageProcessorsManagerTests::testAddProcessor() {
    // Add a new processor
    auto processor = new MockImageProcessor("NewProcessor", "Ctrl+N", "A new image processor", ImageProcessorType::Filter);
    manager->addProcessor(processor);

    // Verify that the processor was added successfully
    AImageProcessor *found = manager->findProcessor("NewProcessor");
    QVERIFY(found != nullptr);
    QCOMPARE(found->name(), QString("NewProcessor"));
}

void ImageProcessorsManagerTests::testRemoveProcessor() {
    // Add a processor to be removed
    auto processor = new MockImageProcessor("RemovableProcessor", "Ctrl+R", "A removable processor", ImageProcessorType::Comparator);
    manager->addProcessor(processor);

    // Verify that the processor exists initially
    AImageProcessor *found = manager->findProcessor("RemovableProcessor");
    QVERIFY(found != nullptr);

    manager->removeProcessor("RemovableProcessor");

    // Verify that the processor was removed successfully
    AImageProcessor *notFound = manager->findProcessor("RemovableProcessor");
    QVERIFY(notFound == nullptr);
}

void ImageProcessorsManagerTests::testAllProcessorsInfo() {
    // Add some processors
    auto processor1 = new MockImageProcessor("InfoProcessor1", "Ctrl+1", "Description 1", ImageProcessorType::Comparator);
    auto processor2 = new MockImageProcessor("InfoProcessor2", "Ctrl+2", "Description 2", ImageProcessorType::Filter);
    manager->addProcessor(processor1);
    manager->addProcessor(processor2);

    QList<ImageProcessorInfo> infos = manager->allProcessorsInfo();

    // Verify that the processors' info is correct
    bool found1 = false, found2 = false;
    for (const auto &info : infos) {
        if (info.name == "InfoProcessor1" && info.hotkey == "Ctrl+1" && info.description == "Description 1") {
            found1 = true;
        }
        if (info.name == "InfoProcessor2" && info.hotkey == "Ctrl+2" && info.description == "Description 2") {
            found2 = true;
        }
    }

    QVERIFY(found1);
    QVERIFY(found2);
}
