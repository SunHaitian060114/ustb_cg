#include "booksearchengine.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

BookSearchEngine::BookSearchEngine(QWidget *parent) : QMainWindow(parent)
{
    initUI();
    loadBooks();
}

void BookSearchEngine::initUI()
{
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    QHBoxLayout *searchLayout = new QHBoxLayout();
    searchInput = new QLineEdit();
    searchInput->setPlaceholderText("Enter character or place name");
    searchButton = new QPushButton("Search");
    searchLayout->addWidget(searchInput);
    searchLayout->addWidget(searchButton);

    resultList = new QListWidget();
    resultList->setSelectionMode(QAbstractItemView::SingleSelection);

    // 设置结果列表标题
    QStringList headerLabels;
    headerLabels << "序号" << "人名/地名" << "页码" << "章节" << "书名";
    QListWidgetItem *header = new QListWidgetItem();
    header->setText(QString("%1\t%2\t%3\t%4\t%5")
                        .arg(headerLabels[0], -10)
                        .arg(headerLabels[1], -20)
                        .arg(headerLabels[2], -10)
                        .arg(headerLabels[3], -10)
                        .arg(headerLabels[4], -30));
    resultList->addItem(header);

    displayArea = new QTextEdit();
    displayArea->setReadOnly(true);

    mainLayout->addLayout(searchLayout);
    mainLayout->addWidget(resultList);
    mainLayout->addWidget(displayArea);

    setCentralWidget(centralWidget);

    connect(searchButton, &QPushButton::clicked, this, &BookSearchEngine::searchClicked);
    connect(searchInput, &QLineEdit::returnPressed, this, &BookSearchEngine::searchClicked);
    connect(resultList, &QListWidget::itemClicked, this, &BookSearchEngine::resultSelected);
}

int BookSearchEngine::calculatePageNumber(int paragraphCount) const
{
    // 假设每10个段落为一页
    return paragraphCount / 10 + 1;
}

void BookSearchEngine::loadBooks()
{
    bookFiles = {
        book_1_path,
        book_2_path,
        book_3_path,
        book_4_path,
        book_5_path,
        book_6_path,
        book_7_path,
        book_8_path
    };

    for (size_t i = 0; i < bookFiles.size(); ++i) {
        Book book;
        book.title = "Harry Potter Book " + std::to_string(i + 1);
        loadBookContent(bookFiles[i], book);
        books.push_back(book);
    }
}

void BookSearchEngine::loadBookContent(const std::string &filename, Book &book)
{
    QFile file(QString::fromStdString(filename));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    QTextStream in(&file);
    std::string content = in.readAll().toStdString();
    file.close();

    parseChapter(content, book);
}

void BookSearchEngine::parseChapter(const std::string &content, Book &book)
{
    std::istringstream iss(content);
    std::string line;
    std::string currentParagraph;
    std::vector<std::string> chapterParagraphs;
    std::vector<int> chapterPages;

    int chapterNumber = 1;
    bool inChapter = false;

    while (std::getline(iss, line)) {
        // 简单的章节检测
        if (line.find("CHAPTER") != std::string::npos ||
            line.find("Chapter") != std::string::npos) {
            if (!chapterParagraphs.empty()) {
                book.paragraphs.push_back(chapterParagraphs);
                book.pageNumbers.push_back(chapterPages);
                book.chapters.push_back("Chapter " + std::to_string(chapterNumber++));
                chapterParagraphs.clear();
                chapterPages.clear();
            }
            inChapter = true;
            continue;
        }

        if (line.empty()) {
            if (!currentParagraph.empty()) {
                chapterParagraphs.push_back(currentParagraph);
                chapterPages.push_back(calculatePageNumber(chapterParagraphs.size()));
                currentParagraph.clear();
            }
        } else {
            if (!currentParagraph.empty()) {
                currentParagraph += " ";
            }
            currentParagraph += line;
        }
    }

    if (!currentParagraph.empty()) {
        chapterParagraphs.push_back(currentParagraph);
        chapterPages.push_back(calculatePageNumber(chapterParagraphs.size()));
    }

    if (!chapterParagraphs.empty()) {
        book.paragraphs.push_back(chapterParagraphs);
        book.pageNumbers.push_back(chapterPages);
        book.chapters.push_back("Chapter " + std::to_string(chapterNumber));
    }
}

std::vector<SearchResult> BookSearchEngine::searchKeyword(const std::string &keyword)
{
    std::vector<SearchResult> results;
    std::string lowerKeyword = keyword;
    std::transform(lowerKeyword.begin(), lowerKeyword.end(), lowerKeyword.begin(), ::tolower);

    // 搜索书名和文件名
    for (size_t b = 0; b < books.size(); ++b) {
        std::string title = books[b].title;
        std::string path = bookFiles[b];
        std::string lowerTitle = title;
        std::string lowerPath = path;
        std::transform(lowerTitle.begin(), lowerTitle.end(), lowerTitle.begin(), ::tolower);
        std::transform(lowerPath.begin(), lowerPath.end(), lowerPath.begin(), ::tolower);

        if (lowerTitle.find(lowerKeyword) != std::string::npos ||
            lowerPath.find(lowerKeyword) != std::string::npos) {
            SearchResult r;
            r.index = results.size() + 1;
            r.keyword = keyword;
            r.bookIndex = b;
            r.chapter = 0;
            r.paragraph = 0;
            r.page = 1;
            r.context = "[Book matched by title or filename]\n\n" + books[b].title;
            results.push_back(r);
        }
    }

    // 搜索正文内容
    for (size_t b = 0; b < books.size(); ++b) {
        for (size_t c = 0; c < books[b].chapters.size(); ++c) {
            for (size_t p = 0; p < books[b].paragraphs[c].size(); ++p) {
                std::string paragraph = books[b].paragraphs[c][p];
                std::string lowerParagraph = paragraph;
                std::transform(lowerParagraph.begin(), lowerParagraph.end(), lowerParagraph.begin(), ::tolower);

                if (lowerParagraph.find(lowerKeyword) != std::string::npos) {
                    SearchResult r;
                    r.index = results.size() + 1;
                    r.keyword = keyword;
                    r.bookIndex = b;
                    r.chapter = c;
                    r.paragraph = p;
                    r.page = books[b].pageNumbers[c][p];
                    r.context = paragraph;
                    results.push_back(r);
                }
            }
        }
    }

    return results;
}

std::string BookSearchEngine::getContext(const SearchResult &result)
{
    if (result.context.find("[Book matched") != std::string::npos) {
        return result.context;
    }

    std::string context;
    Book &book = books[result.bookIndex];
    int chapter = result.chapter;
    int paragraph = result.paragraph;

    if (paragraph > 0) {
        context += "[Previous Paragraph] (Page " + std::to_string(book.pageNumbers[chapter][paragraph-1]) + ")\n";
        context += book.paragraphs[chapter][paragraph - 1] + "\n\n";
    }

    context += "[Current Paragraph] (Page " + std::to_string(result.page) + ")\n";
    context += book.paragraphs[chapter][paragraph] + "\n\n";

    if (paragraph + 1 < book.paragraphs[chapter].size()) {
        context += "[Next Paragraph] (Page " + std::to_string(book.pageNumbers[chapter][paragraph+1]) + ")\n";
        context += book.paragraphs[chapter][paragraph + 1] + "\n";
    }

    context += "\n---\n";
    context += "Book: " + book.title + "\n";
    context += "Chapter: " + book.chapters[chapter] + "\n";
    context += "Page: " + std::to_string(result.page) + "\n";
    context += "Paragraph: " + std::to_string(paragraph + 1) + "\n";

    return context;
}

void BookSearchEngine::searchClicked()
{
    QString keyword = searchInput->text().trimmed();
    if (keyword.isEmpty()) {
        displayArea->setText("Please enter a search term");
        return;
    }

    cachedResults = searchKeyword(keyword.toStdString());
    resultList->clear();

    // 重新添加标题
    QStringList headerLabels;
    headerLabels << "序号" << "人名/地名" << "页码" << "章节" << "书名";
    QListWidgetItem *header = new QListWidgetItem();
    header->setText(QString("%1\t%2\t%3\t%4\t%5")
                        .arg(headerLabels[0], -10)
                        .arg(headerLabels[1], -20)
                        .arg(headerLabels[2], -10)
                        .arg(headerLabels[3], -10)
                        .arg(headerLabels[4], -30));
    resultList->addItem(header);

    if (cachedResults.empty()) {
        displayArea->setText("No results found for: " + keyword);
        return;
    }

    for (const auto &r : cachedResults) {
        QString itemText = QString("%1\t%2\t%3\t%4\t%5")
        .arg(r.index, -10)
            .arg(QString::fromStdString(r.keyword), -20)
            .arg(r.page, -10)
            .arg(QString::fromStdString(books[r.bookIndex].chapters[r.chapter]), -10)
            .arg(QString::fromStdString(books[r.bookIndex].title), -30);
        resultList->addItem(itemText);
    }

    displayArea->setText(QString("Found %1 results for: %2").arg(cachedResults.size()).arg(keyword));
}

void BookSearchEngine::resultSelected(QListWidgetItem *item)
{
    // 跳过标题行
    if (item == resultList->item(0)) return;

    int index = item->text().split('\t')[0].toInt() - 1;

    if (index >= 0 && index < cachedResults.size()) {
        std::string context = getContext(cachedResults[index]);
        displayArea->setText(QString::fromStdString(context));
    }
}
