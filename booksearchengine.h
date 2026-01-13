#ifndef BOOKSEARCHENGINE_H
#define BOOKSEARCHENGINE_H

#include "config.h"
#include <QMainWindow>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QListWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFile>
#include <QTextStream>
#include <vector>
#include <string>

struct SearchResult {
    int index;
    std::string keyword;
    int bookIndex;
    int chapter;
    int paragraph;
    int page;  // 新增页码字段
    std::string context;
};

struct Book {
    std::string title;
    std::vector<std::string> chapters;
    std::vector<std::vector<std::string>> paragraphs;
    std::vector<std::vector<int>> pageNumbers;  // 新增页码映射
};

class BookSearchEngine : public QMainWindow
{
    Q_OBJECT

public:
    explicit BookSearchEngine(QWidget *parent = nullptr);
    std::vector<std::string> bookFiles;
    std::vector<SearchResult> cachedResults;

private slots:
    void searchClicked();
    void resultSelected(QListWidgetItem *item);

private:
    void initUI();
    void loadBooks();
    std::vector<SearchResult> searchKeyword(const std::string &keyword);
    std::string getContext(const SearchResult &result);
    void loadBookContent(const std::string &filename, Book &book);
    void parseChapter(const std::string &content, Book &book);
    int calculatePageNumber(int paragraphCount) const;

    std::vector<Book> books;
    QTextEdit *displayArea;
    QLineEdit *searchInput;
    QPushButton *searchButton;
    QListWidget *resultList;
};

#endif // BOOKSEARCHENGINE_H
