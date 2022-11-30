#ifndef DOCKCONTROLLER_HPP
#define DOCKCONTROLLER_HPP

#pragma once

#include <list>

#include <QObject>
#include <QPointer>
#include <QVariant>

class DockItemData {
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
  bool isAdsorbed() const;
  void setAdsorbed(bool adsorbed);

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
};





class DockContextData {
public:
  explicit DockContextData(QPointer<QObject> context = nullptr);

public:
  QPointer<QObject> getContext() const;
  void setContext(QPointer<QObject> context);

  bool isVaild() const;
  operator bool() const;

public:
  int getAdsorbedX() const;
  int getAdsorbedY() const;
  int getAdsorbedWidth() const;
  int getAdsorbedHeight() const;

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
  Q_PROPERTY(QObjectList itemList READ getItemList NOTIFY sigItemListChanged);
  Q_INVOKABLE QObjectList getItemList() const;
  Q_SIGNAL void sigItemListChanged(const QObjectList& item_list);

  Q_INVOKABLE bool isItemExist(QObject* item) const;
  Q_INVOKABLE void appendItem(QObject* item);
  Q_INVOKABLE void removeItem(QObject* item);
  Q_INVOKABLE void focusItem(QObject* item);
  Q_INVOKABLE int countItem() const;

  Q_INVOKABLE QObject* findItemByTitle(const QString& title);

public:
  Q_PROPERTY(QStringList contextTitleList READ getContextTitleList NOTIFY sigContextTitleListChanged);
  Q_INVOKABLE QStringList getContextTitleList() const;
  Q_SIGNAL void sigContextTitleListChanged(const QStringList& context_title_list);

public:
  Q_PROPERTY(bool contextVisible READ isContextVisible NOTIFY sigContextVisibleChanged);
  Q_INVOKABLE bool isContextVisible() const;
  Q_SIGNAL void sigContextVisibleChanged(bool visible);

  Q_INVOKABLE void syncContext();

private:
  std::pair<bool, DockItemData&> findDataByItem(QObject* item);

  Q_SLOT void onContextDestroyed();
  Q_SLOT void onItemDestroyed(QObject* item);

private:
  DockContextData context_;
  std::list<DockItemData> item_list_;
};

#endif // DOCKCONTROLLER_HPP
