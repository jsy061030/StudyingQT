#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QDialog>
#include <QtCore>
#include <QMainWindow>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtGui/QIcon>

void qDialog(QString& i)
{
    QDialog dialog;
    dialog.setWindowTitle("Hello, dialog!");
    QLabel* dialogLabel = new QLabel(i, &dialog);
    dialogLabel->setAlignment(Qt::AlignCenter);
    QVBoxLayout* dialogLayout = new QVBoxLayout(&dialog);
    dialogLayout->addWidget(dialogLabel);
    dialog.exec();
}

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    QMainWindow window;
    window.resize(500, 300);
    window.setWindowTitle("Nice to meet you!");
    window.setWindowIcon(QIcon("C:\\Users\\蒋尚佑\\Pictures\\quniang\\308301 #2851112.png"));
    QWidget* centralWidget = new QWidget(&window);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    QLabel* label = new QLabel("Hello World!", centralWidget);
    QTextEdit* textEdit = new QTextEdit(&window);
    label->setAlignment(Qt::AlignCenter);
    QPushButton* pushButton = new QPushButton("Click Me!", centralWidget);
    QObject::connect(pushButton, &QPushButton::clicked, [&]() {
        qDebug() << "Button clicked! Opening dialog...";
        QString inputStr = textEdit->toPlainText();
        qDialog(inputStr);
        });
    mainLayout->addWidget(label);
    mainLayout->addWidget(textEdit);
    mainLayout->addWidget(pushButton);
    mainLayout->setAlignment(Qt::AlignCenter);
    window.setCentralWidget(centralWidget);

    window.show();
    return app.exec();
}