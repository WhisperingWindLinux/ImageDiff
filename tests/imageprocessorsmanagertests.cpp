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
    auto processor = make_shared<MockImageProcessor>("MockProcessor", "Ctrl+M", "A mock image processor", ImageProcessorType::Comparator);
    manager->addProcessor(processor);

    // Verify that the processor can be found by name
    auto found = manager->findProcessorByName("MockProcessor");
    QVERIFY(found != nullptr);
    QCOMPARE(found->name(), QString("MockProcessor"));

    // Verify that searching for a non-existent processor returns nullptr
    auto notFound = manager->findProcessorByName("NonExistentProcessor");
    QVERIFY(notFound == nullptr);
}

void ImageProcessorsManagerTests::testAddProcessor() {
    // Add a new processor
    auto processor = make_shared<MockImageProcessor>("NewProcessor", "Ctrl+N", "A new image processor", ImageProcessorType::Filter);
    manager->addProcessor(processor);

    // Verify that the processor was added successfully
    auto found = manager->findProcessorByName("NewProcessor");
    QVERIFY(found != nullptr);
    QCOMPARE(found->name(), QString("NewProcessor"));
}

void ImageProcessorsManagerTests::testRemoveProcessor() {
    // Add a processor to be removed
    auto processor = make_shared<MockImageProcessor>("RemovableProcessor", "Ctrl+R", "A removable processor", ImageProcessorType::Comparator);
    manager->addProcessor(processor);

    // Verify that the processor exists initially
    auto found = manager->findProcessorByName("RemovableProcessor");
    QVERIFY(found != nullptr);

    manager->removeProcessor("RemovableProcessor");

    // Verify that the processor was removed successfully
    auto notFound = manager->findProcessorByName("RemovableProcessor");
    QVERIFY(notFound == nullptr);
}

void ImageProcessorsManagerTests::testAllProcessorsInfo() {
    // Add some processors
    auto processor1 = make_shared<MockImageProcessor>("InfoProcessor1", "Ctrl+1", "Description 1", ImageProcessorType::Comparator);
    auto processor2 = make_shared<MockImageProcessor>("InfoProcessor2", "Ctrl+2", "Description 2", ImageProcessorType::Filter);
    manager->addProcessor(processor1);
    manager->addProcessor(processor2);

    QList<ImageProcessorInfo> infos = manager->allProcessorsInfo();

    // Verify that the processors' info is correct
    bool found1 = false, found2 = false;
    foreach (auto info, infos) {
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

void ImageProcessorsManagerTests::testAllProcessorsInfoTheSameHotkeys() {
    {
    auto processor1 = make_shared<MockImageProcessor>("Processor1", "Ctrl+A", "Description 1", ImageProcessorType::Comparator);
    auto processor2 = make_shared<MockImageProcessor>("Processor2", "Ctrl+B", "Description 2", ImageProcessorType::Comparator);
    auto processor3 = make_shared<MockImageProcessor>("Processor3", "Ctrl+A", "Description 3", ImageProcessorType::Comparator);
    auto processor4 = make_shared<MockImageProcessor>("Processor4", "Ctrl+C", "Description 4", ImageProcessorType::Comparator);

    manager->addProcessor(processor1);
    manager->addProcessor(processor2);
    manager->addProcessor(processor3);
    manager->addProcessor(processor4);

    QList<ImageProcessorInfo> result = manager->allProcessorsInfo();

    QCOMPARE(result.size(), 4);

    QCOMPARE(result[0].hotkey, QString(""));
    QCOMPARE(result[1].hotkey, QString("Ctrl+B"));
    QCOMPARE(result[2].hotkey, QString(""));
    QCOMPARE(result[3].hotkey, QString("Ctrl+C"));
    }

    {
        auto processor1 = make_shared<MockImageProcessor>("Processor1", "Ctrl+A", "Description 1", ImageProcessorType::Comparator);
        auto processor2 = make_shared<MockImageProcessor>("Processor2", "Ctrl+B", "Description 2", ImageProcessorType::Comparator);
        auto processor3 = make_shared<MockImageProcessor>("Processor3", "Ctrl+a", "Description 3", ImageProcessorType::Comparator);
        auto processor4 = make_shared<MockImageProcessor>("Processor4", "Ctrl+C", "Description 4", ImageProcessorType::Comparator);

        manager->addProcessor(processor1);
        manager->addProcessor(processor2);
        manager->addProcessor(processor3);
        manager->addProcessor(processor4);

        QList<ImageProcessorInfo> result = manager->allProcessorsInfo();

        QCOMPARE(result.size(), 4);

        QCOMPARE(result[0].hotkey, QString(""));
        QCOMPARE(result[1].hotkey, QString("Ctrl+B"));
        QCOMPARE(result[2].hotkey, QString(""));
        QCOMPARE(result[3].hotkey, QString("Ctrl+C"));
    }

    {
        auto processor1 = make_shared<MockImageProcessor>("Processor1", "Ctrl+A", "Description 1", ImageProcessorType::Filter);
        auto processor2 = make_shared<MockImageProcessor>("Processor2", "Ctrl+B", "Description 2", ImageProcessorType::Filter);
        auto processor3 = make_shared<MockImageProcessor>("Processor3", "Ctrl+A", "Description 3", ImageProcessorType::Filter);
        auto processor4 = make_shared<MockImageProcessor>("Processor4", "Ctrl+C", "Description 4", ImageProcessorType::Filter);

        manager->addProcessor(processor1);
        manager->addProcessor(processor2);
        manager->addProcessor(processor3);
        manager->addProcessor(processor4);

        QList<ImageProcessorInfo> result = manager->allProcessorsInfo();

        QCOMPARE(result.size(), 4);

        QCOMPARE(result[0].hotkey, QString(""));
        QCOMPARE(result[1].hotkey, QString("Ctrl+B"));
        QCOMPARE(result[2].hotkey, QString(""));
        QCOMPARE(result[3].hotkey, QString("Ctrl+C"));
    }

    {
        auto processor1 = make_shared<MockImageProcessor>("Processor1", "Ctrl+A", "Description 1", ImageProcessorType::Filter);
        auto processor2 = make_shared<MockImageProcessor>("Processor2", "Ctrl+B", "Description 2", ImageProcessorType::Filter);
        auto processor3 = make_shared<MockImageProcessor>("Processor3", "Ctrl+A", "Description 3", ImageProcessorType::Comparator);
        auto processor4 = make_shared<MockImageProcessor>("Processor4", "Ctrl+C", "Description 4", ImageProcessorType::Comparator);

        manager->addProcessor(processor1);
        manager->addProcessor(processor2);
        manager->addProcessor(processor3);
        manager->addProcessor(processor4);

        QList<ImageProcessorInfo> result = manager->allProcessorsInfo();

        QCOMPARE(result.size(), 4);

        QCOMPARE(result[0].hotkey, QString(""));
        QCOMPARE(result[1].hotkey, QString("Ctrl+B"));
        QCOMPARE(result[2].hotkey, QString(""));
        QCOMPARE(result[3].hotkey, QString("Ctrl+C"));
    }
}


