#include <include/mainwindow.h>
#include <include/interactive_view.h>
#include <include/hs_triangulation.h>


#include <QMenuBar>
#include <QMenu>
#include <QAction>

#include <QTextCodec>
#include <Windows.h>

#include <QToolButton>

#include <QSplitter>



#include <cstdlib>

#include <string>


static std::string rs = R"(Welcome to the urban geometry app!

Here you get to test and understand some fundamental algorithms used in urban analysis and how the underlying C++ computer code works!

We're starting with Delaunay Triangulations (these allow us to check the Voronoi Diagram) and Constrained Delaunay Triangulations (these allow us to find the Shortest Paths).
)";

int main(int argc, char* argv[])
{
    qDebug() << "Main";
    QApplication app(argc, argv);

    QFont font("Segoe UI", 10); // You can choose a font that supports Japanese
    app.setFont(font);

    QLocale::setDefault(QLocale(QLocale::Japanese, QLocale::Japan));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("Shift-JIS"));

    QDir currentDir(QCoreApplication::applicationDirPath());
    QString iconPath = currentDir.filePath("icon_windows.ico");

    qDebug() << "icon path" << iconPath;

    MainWindow mainWindow(nullptr, &app);
    mainWindow.setGeometry(100, 100, 1600, 800);
    mainWindow.setWindowTitle("Urban Geometry");
    mainWindow.setWindowIcon(QIcon(iconPath));

    UINT codePage = GetACP();
    // Output the code page
    qDebug() << "System Encoding Code Page: " << codePage;

    QTextCodec* codec = QTextCodec::codecForLocale();
    qDebug() << "Current locale encoding: " << codec->name();

    // Create a Delaunay triangulation
    Delaunay dt;
    CDT cdt;

    InteractiveView_hs_triangulation view(cdt, dt, mainWindow, &mainWindow);

    // Create a splitter to allow resizing
    QSplitter* splitter = new QSplitter(Qt::Vertical, &mainWindow);
    splitter->addWidget(&view); // Add the main view to the splitter
    splitter->addWidget(&view.textEditL); // Add the textEditL to the splitter

    // Set the initial sizes (optional)
    QList<int> sizes;
    sizes << 600 << 200; // Adjust these values as needed
    splitter->setSizes(sizes);

    // Set the splitter as the central widget
    mainWindow.setCentralWidget(splitter);

    view.textEditL.setHidden(false); // Ensure the text box is visible

    mainWindow.show();
    mainWindow.fit_view();

    QString currentFilePath = __FILE__;
    QFileInfo fileInfo(currentFilePath);
    QString sourceDir = fileInfo.absolutePath();
    qDebug() << "Source Directory:" << sourceDir;

    QString s = QString::fromStdString(rs);

    // Create a QMessageBox
    QMessageBox msgBox;
    msgBox.setWindowTitle("Welcome!");

    // Remove the default text area of the QMessageBox to make room for custom content
    msgBox.setText("");  // Keep this empty, we'll use custom labels

    // Create a label to display the image
    QLabel* imageLabel = new QLabel(&msgBox);
    QString imagePath = sourceDir + "/welcome_image.png";
    QPixmap pixmap(imagePath);

    // Check if the image is successfully loaded
    if (pixmap.isNull()) {
        qDebug() << "Failed to load image at:" << imagePath;
    }
    else {
        imageLabel->setPixmap(pixmap);
        imageLabel->setAlignment(Qt::AlignCenter); // Center the image
    }

    // Create a text label for the message
    QLabel* textLabel = new QLabel(s, &msgBox);
    textLabel->setAlignment(Qt::AlignCenter); // Center the text
    textLabel->setWordWrap(true); // Wrap the text if it's too long

    // Create a vertical layout and add the image and text labels
    QVBoxLayout* customLayout = new QVBoxLayout;
    customLayout->addWidget(imageLabel);
    customLayout->addWidget(textLabel);

    // Create a QWidget to contain the custom layout and set it as the custom content for the message box
    QWidget* customWidget = new QWidget(&msgBox);
    customWidget->setLayout(customLayout);

    // Add the custom content to the message box
    msgBox.layout()->addWidget(customWidget);

    // Ensure the QMessageBox resizes to fit the content
    msgBox.setMinimumSize(pixmap.width() + 20, pixmap.height() + textLabel->sizeHint().height() + 100);

    // Show the message box
    msgBox.exec();



    QMessageBox::information(nullptr, "Colaborate!", "This is an open project, so any problems, comments or suggestions, please send them through the direct help email above!");

    QMessageBox::information(nullptr, "Delaunay Triangulation", mainWindow.ptr_iv->message_qs_DT_start);

    return app.exec();
}
