#include "booksearchengine.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    BookSearchEngine engine;
    engine.setWindowTitle("Harry Potter Book Search Engine");
    engine.resize(1000, 800);
    engine.show();

    return app.exec();
}
