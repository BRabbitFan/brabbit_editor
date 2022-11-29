#ifndef DOCKLISTMODEL_HPP
#define DOCKLISTMODEL_HPP

#pragma once

#include <list>

#include <QAbstractListModel>
#include <QPointer>

class DockController;

class DockItemData {
  friend class DockController;

public:
  explicit DockItemData(QPointer<QObject> item = nullptr);

public:
  QPointer<QObject> getItem() const;
  void setItem(QPointer<QObject> item);

  bool isVaild() const;
  operator bool() const;

  bool isEqual(const DockItemData& that) const;
  bool operator==(const DockItemData& that) const;

public:
  bool isMaximum() const;
  void setMaximum(bool maximum);

  bool isVisible() const;
  void setVisible(bool visible);

  int getX() const;
  void setX(int x);

  int getY() const;
  void setY(int y);

  int getWidth() const;
  void setWidth(int width);

  int getHeight() const;
  void setHeight(int height);

  QByteArray getTtile() const;
  void setTitle(const QByteArray& title);

private:
  QPointer<QObject> item_;

  struct {
    int x = -1;
    int y = -1;
    int w = -1;
    int h = -1;

    operator bool() const {
      return x != -1 &&
             y != -1 &&
             w != -1 &&
             h != -1;
    }

    void clear() {
      x = -1;
      y = -1;
      w = -1;
      h = -1;
    }

  } cache_;
};





class DockContextData {
  friend class DockController;

public:
  explicit DockContextData(QPointer<QObject> context = nullptr);

public:
  QPointer<QObject> getContext() const;
  void setContext(QPointer<QObject> context);

  bool isVaild() const;
  operator bool() const;

public:
  int getX() const;
  int getY() const;
  int getMaxWidth() const;
  int getMaxHeight() const;

private:
  QPointer<QObject> context_;
};





class DockController : public QObject {
  Q_OBJECT;

public:
  explicit DockController(QObject* parent = nullptr);

public:
  Q_PROPERTY(QObject* context READ getContext WRITE setContext NOTIFY sigContextChanged);
  Q_INVOKABLE QObject* getContext() const;
  Q_SLOT void setContext(QObject* context);
  Q_SIGNAL void sigContextChanged(QObject* context);

public:
  Q_PROPERTY(QObjectList itemList READ getItemList NOTIFY sigItemListChanged CONSTANT);
  Q_INVOKABLE QObjectList getItemList() const;
  Q_SIGNAL void sigItemListChanged(const QObjectList& item_list);

  Q_INVOKABLE bool isItemExist(QObject* item) const;
  Q_INVOKABLE void appendItem(QObject* item);
  Q_INVOKABLE void removeItem(QObject* item);
  Q_INVOKABLE void focusItem(QObject* item);
  Q_INVOKABLE int countItem() const;

  Q_INVOKABLE QObject* findItemByTitle(const QString& title);

public:
  Q_PROPERTY(QStringList titleList READ getTitleList NOTIFY sigTitleListChanged CONSTANT);
  Q_INVOKABLE QStringList getTitleList() const;
  Q_SIGNAL void sigTitleListChanged(const QStringList& title_list);

private:
  DockItemData findDataByItem(QObject* item);
  Q_SLOT void onItemMaximumChanged(bool maximum);

private:
  DockContextData context_;
  std::list<DockItemData> item_list_;
};

#endif // DOCKLISTMODEL_HPP
