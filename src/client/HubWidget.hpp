/*
** EPITECH PROJECT, 2021
** B-CPP-500-NCE-5-1-babel-lionel.da-rocha-da-silva
** File description:
** HubWidget
*/

#ifndef HUBWIDGET_HPP_
#define HUBWIDGET_HPP_

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <vector>

class HubWidget : public QWidget {
    Q_OBJECT
    public:
        HubWidget(QWidget *parent = 0);
        ~HubWidget();

        QPushButton *getButton() const;
        int getSelected() const;
        std::string getSelectedName() const;
        void setUsername(const std::string username);

        void addUser(std::string username);
        void removeUser(std::string username);

    signals:
    public slots:
    private slots:
        void onListItemClicked(QListWidgetItem* item);

    protected:
    private:
        QPushButton *_callButton;
        QListWidget *_userList;
        QLabel *_labelUsername;
        int _selected;
};

#endif /* !HUBWIDGET_HPP_ */
