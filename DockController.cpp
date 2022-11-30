#include "DockListModel.hpp"

#include <QDebug>

namespace key {

constexpr auto ADSORBED       { "adsorbed"       };
constexpr auto VISIBLE        { "visible"        };

constexpr auto X              { "x"              };
constexpr auto Y              { "y"              };
constexpr auto WIDTH          { "width"          };
constexpr auto HEIGHT         { "height"         };

constexpr auto ADSORBED_X     { "adsorbedX"      };
constexpr auto ADSORBED_Y     { "adsorbedY"      };
constexpr auto ADSORBED_WIDTH { "adsorbedWidth"  };
constexpr auto ADSORBED_HEIGHT{ "adsorbedHeight" };

constexpr auto TITLE          { "title"          };

}

DockItemData::DockItemData(QPointer<QObject> item) : item_(item) {}

QPointer<QObject> DockItemData::getItem() const { return item_; }

void DockItemData::setItem(QPointer<QObject> item) { item_ = item; }

bool DockItemData::isVaild() const { return item_ != nullptr; }

DockItemData::operator bool() const { return isVaild(); }

bool DockItemData::isEqual(const DockItemData& that) const { return this->item_ == that.item_; }

bool DockItemData::operator==(const DockItemData& that) const { return isEqual(that); }

bool DockItemData::isAdsorbed() const {
  if (!isVaild()) { return false; }
  return item_->property(key::ADSORBED).toBool();
}

void DockItemData::setAdsorbed(bool adsorbed) {
  if (!isVaild()) { return; }
  item_->setProperty(key::ADSORBED, adsorbed);
}

bool DockItemData::isVisible() const {
  if (!isVaild()) { return false; }
  return item_->property(key::VISIBLE).toBool();
}

void DockItemData::setVisible(bool visible) {
  if (!isVaild()) { return; }
  item_->setProperty(key::VISIBLE, visible);
}

int DockItemData::getX() const {
  if (!isVaild()) { return false; }
  return item_->property(key::X).toInt();
}

void DockItemData::setX(int x) {
  if (!isVaild()) { return; }
  item_->setProperty(key::X, x);
}

int DockItemData::getY() const {
  if (!isVaild()) { return false; }
  return item_->property(key::Y).toInt();
}

void DockItemData::setY(int y) {
  if (!isVaild()) { return; }
  item_->setProperty(key::Y, y);
}

int DockItemData::getWidth() const {
  if (!isVaild()) { return false; }
  return item_->property(key::WIDTH).toInt();
}

void DockItemData::setWidth(int width) {
  if (!isVaild()) { return; }
  item_->setProperty(key::WIDTH, width);
}

int DockItemData::getHeight() const {
  if (!isVaild()) { return false; }
  return item_->property(key::HEIGHT).toInt();
}

void DockItemData::setHeight(int height) {
  if (!isVaild()) { return; }
  item_->setProperty(key::HEIGHT, height);
}

QByteArray DockItemData::getTtile() const {
  if (!isVaild()) { return {}; }
  return item_->property(key::TITLE).toByteArray();
}

void DockItemData::setTitle(const QByteArray& title) {
  if (!isVaild()) { return; }
  item_->setProperty(key::TITLE, title);
}





DockContextData::DockContextData(QPointer<QObject> context) : context_(context) {}

QPointer<QObject> DockContextData::getContext() const { return context_; }

void DockContextData::setContext(QPointer<QObject> context) { context_ = context; }

bool DockContextData::isVaild() const { return context_ != nullptr; }

DockContextData::operator bool() const { return isVaild(); }

int DockContextData::getAdsorbedX() const {
  if (!isVaild()) { return false; }
  return context_->property(key::ADSORBED_X).toInt();
}

int DockContextData::getAdsorbedY() const {
  if (!isVaild()) { return false; }
  return context_->property(key::ADSORBED_Y).toInt();
}

int DockContextData::getAdsorbedWidth() const {
  if (!isVaild()) { return false; }
  return context_->property(key::ADSORBED_WIDTH).toInt();
}

int DockContextData::getAdsorbedHeight() const {
  if (!isVaild()) { return false; }
  return context_->property(key::ADSORBED_HEIGHT).toInt();
}





DockController::DockController(QObject* parent) : QObject(parent) {}

QObject* DockController::getContext() const {
  return context_.getContext().get();
}

void DockController::setContext(QObject* context) {
  QPointer<QObject> context_ptr{ context };
  if (context_.getContext() == context_ptr) {
    return;
  }

  if (context_) {
    context_.getContext()->disconnect(this);
    this->disconnect(context_.getContext());
  }

  connect(context, &QObject::destroyed, this, &DockController::onItemDestroyed);

  context_.setContext(context_ptr);
  item_list_.clear();

  Q_EMIT sigContextChanged(context_ptr.get());
  Q_EMIT sigItemListChanged(getItemList());
}

QObjectList DockController::getItemList() const {
  QObjectList result;
  for (const auto& data : item_list_) {
    result.emplaceBack(data.getItem().get());
  }
  return result;
}

bool DockController::isItemExist(QObject* item) const {
  DockItemData item_data{ item };
  for (const auto& list_data : item_list_) {
    if (list_data == item_data) {
      return true;
    }
  }

  return false;
}

void DockController::appendItem(QObject* item) {
  if (!item) { return; }
  if (isItemExist(item)) { return; }

  item_list_.emplace_back(DockItemData{ item });

  connect(item, &QObject::destroyed, this, &DockController::onItemDestroyed);

  Q_EMIT sigContextTitleListChanged(getContextTitleList());
  Q_EMIT sigContextVisibleChanged(isContextVisible());
}

void DockController::removeItem(QObject* item) {
  if (!item) { return; }
  if (!isItemExist(item)) { return; }

  DockItemData data{ item };

  data.setAdsorbed(false);
  item_list_.remove(data);

  item->disconnect(this);
  this->disconnect(item);

  Q_EMIT sigContextTitleListChanged(getContextTitleList());
  Q_EMIT sigContextVisibleChanged(isContextVisible());
}

void DockController::focusItem(QObject* item) {
  if (!item) { return; }
  if (!isItemExist(item)) { return; }

  DockItemData data{ item };
  for (auto& list_data : item_list_) {
    if (!list_data.isAdsorbed()) { continue; }
    const bool focused = list_data == data;

    list_data.setVisible(focused);

    if (focused) {
      data.setX(context_.getAdsorbedX());
      data.setY(context_.getAdsorbedY());
      data.setWidth(context_.getAdsorbedWidth());
      data.setHeight(context_.getAdsorbedHeight());
    }
  }
}

int DockController::countItem() const { return static_cast<int>(item_list_.size()); }

QObject* DockController::findItemByTitle(const QString& title) {
  for (const auto& data : item_list_) {
    if (data.getTtile() == title) {
      return data.getItem().get();
    }
  }

  return nullptr;
}

QStringList DockController::getContextTitleList() const {
  QStringList result;

  for (const auto& data : item_list_) {
    if (!data.isAdsorbed()) { continue; }
    result.emplaceFront(data.getTtile());
  }

  return result;
}

bool DockController::isContextVisible() const {
  for (const auto& data : item_list_) {
    if (data.isAdsorbed()) {
      return true;
    }
  }

  return false;
}

std::pair<bool, DockItemData&> DockController::findDataByItem(QObject* item) {
  DockItemData data{ sender() };
  for (auto& list_data : item_list_) {
    if (list_data == data) {
      return { true, list_data };
    }
  }

  return { false, item_list_.front() };
}

void DockController::onContextDestroyed() {
  for (const auto& data : item_list_) {
    removeItem(data.getItem());
  }
}

void DockController::onItemDestroyed(QObject* item) {
  removeItem(item);
}

void DockController::syncContext() {
  Q_EMIT sigContextTitleListChanged(getContextTitleList());
  Q_EMIT sigContextVisibleChanged(isContextVisible());
}
