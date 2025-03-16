#include "recentfilespathformatertest.h"
#include <presentation/presenters/recentfilespresenter.h>

void RecentFilesPathFormatorTest::testPairToString_ValidInput() {
    QPair<QString, QString> input("path/to/file1", "path/to/file2");
    QString expectedOutput = "path/to/file1 -> path/to/file2";

    QCOMPARE(RecentFilesPresenter::pairToString(input), expectedOutput);
}

void RecentFilesPathFormatorTest::testPairToString_EmptyInput() {
    QPair<QString, QString> input("", "path/to/file2");
    QString expectedOutput = ""; // Empty string expected

    QCOMPARE(RecentFilesPresenter::pairToString(input), expectedOutput);
}

void RecentFilesPathFormatorTest::testStringToPair_ValidInput() {
    QString input = "path/to/file1 -> path/to/file2";
    QPair<QString, QString> expectedOutput("path/to/file1", "path/to/file2");

    auto result = RecentFilesPresenter::stringToPair(input);
    QVERIFY(result.has_value());
    QCOMPARE(result.value(), expectedOutput);
}

void RecentFilesPathFormatorTest::testStringToPair_InvalidInput() {
    QString input = "invalid_format_string";

    auto result = RecentFilesPresenter::stringToPair(input);
    QVERIFY(!result.has_value()); // Expect no value (std::nullopt)
}

void RecentFilesPathFormatorTest::testStringToPair_EmptyString() {
    QString input = "";

    auto result = RecentFilesPresenter::stringToPair(input);
    QVERIFY(!result.has_value()); // Expect no value (std::nullopt)
}
