#include "DockListModel.hpp"

static constexpr auto KEY_MAXIMUM   { "maximum"   };
static constexpr auto KEY_VISIBLE   { "visible"   };
static constexpr auto KEY_WIDTH     { "width"     };
static constexpr auto KEY_HEIGHT    { "height"    };
static constexpr auto KEY_MAX_WIDTH { "maxWidth"  };
static constexpr auto KEY_MAX_HEIGHT{ "maxHeight" };
static constexpr auto KEY_TITLE     { "title"     };
static constexpr auto KEY_X         { "x"         };
static constexpr auto KEY_Y         { "y"         };

DockItemData::DockItemData(QPointer<QObject> item) : item_(item) {}

QPointer<QObject> DockItemData::getItem() const { return item_; }

void DockItemData::setItem(QPointer<QObject> item) { item_ = item; }

bool DockItemData::isVaild() const { return item_ != nullptr; }

DockItemData::operator bool() const { return isVaild(); }

bool DockItemData::isEqual(const DockItemData& that) const { return this->item_ == that.item_; }

bool DockItemData::operator==(const DockItemData& that) const { return isEqual(that); }

bool DockItemData::isMaximum() const {
  if (!isVaild()) { return false; }
  return item_->property(KEY_MAXIMUM).toBool();
}

void DockItemData::setMaximum(bool visible) {
  if (!isVaild()) { return; }
  item_->setProperty(KEY_MAXIMUM, visible);
}

bool DockItemData::isVisible() const {
  if (!isVaild()) { return false; }
  return item_->property(KEY_VISIBLE).toBool();
}

void DockItemData::setVisible(bool visible) {
  if (!isVaild()) { return; }
  item_->setProperty(KEY_VISIBLE, visible);
}

int DockItemData::getX() const {
  if (!isVaild()) { return false; }
  return item_->property(KEY_X).toInt();
}

void DockItemData::setX(int x) {
  if (!isVaild()) { return; }
  item_->setProperty(KEY_X, x);
}

int DockItemData::getY() const {
  if (!isVaild()) { return false; }
  return item_->property(KEY_Y).toInt();
}

void DockItemData::setY(int y) {
  if (!isVaild()) { return; }
  item_->setProperty(KEY_Y, y);
}

int DockItemData::getWidth() const {
  if (!isVaild()) { return false; }
  return item_->property(KEY_WIDTH).toInt();
}

void DockItemData::setWidth(int width) {
  if (!isVaild()) { return; }
  item_->setProperty(KEY_WIDTH, width);
}

int DockItemData::getHeight() const {
  if (!isVaild()) { return false; }
  return item_->property(KEY_HEIGHT).toInt();
}

void DockItemData::setHeight(int height) {
  if (!isVaild()) { return; }
  item_->setProperty(KEY_HEIGHT, height);
}

QByteArray DockItemData::getTtile() const {
  if (!isVaild()) { return {}; }
  return item_->property(KEY_TITLE).toByteArray();
}

void DockItemData::setTitle(const QByteArray& title) {
  if (!isVaild()) { return; }
  item_->setProperty(KEY_TITLE, title);
}




DockContextData::DockContextData(QPointer<QObject> context) : context_(context) {}

QPointer<QObject> DockContextData::getContext() const { return context_; }

void DockContextData::setContext(QPointer<QObject> context) { context_ = context; }

bool DockContextData::isVaild() const { return context_ != nullptr; }

DockContextData::operator bool() const { return isVaild(); }

int DockContextData::getX() const {
  if (!isVaild()) { return false; }
  return context_->property(KEY_X).toInt();
}

int DockContextData::getY() const {
  if (!isVaild()) { return false; }
  return context_->property(KEY_Y).toInt();
}

int DockContextData::getMaxWidth() const {
  if (!isVaild()) { return false; }
  return context_->property(KEY_MAX_WIDTH).toInt();
}

int DockContextData::getMaxHeight() const {
  if (!isVaild()) { return false; }
  return context_->property(KEY_MAX_HEIGHT).toInt();
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

  connect(item, &QObject::destroyed, this, [this, item](){
    removeItem(item);
  });

  connect(item, SIGNAL(sigMaximumChanged(bool)), this, SLOT(onItemMaximumChanged(bool)));

  Q_EMIT sigTitleListChanged(getTitleList());
}

void DockController::removeItem(QObject* item) {
  if (!item) { return; }
  if (!isItemExist(item)) { return; }

  item_list_.remove(DockItemData{ item });

  item->disconnect(this);
  this->disconnect(item);

  Q_EMIT sigTitleListChanged(getTitleList());
}

void DockController::focusItem(QObject* item) {
  if (!item) { return; }
  if (!isItemExist(item)) { return; }

  DockItemData data{ item };
  for (auto& list_data : item_list_) {
    list_data.setVisible(list_data == data);
  }
}

int DockController::countItem() const { return item_list_.size(); }

QObject* DockController::findItemByTitle(const QString& title) {
  for (const auto& data : item_list_) {
    if (data.getTtile() == title) {
      return data.getItem().get();
    }
  }

  return nullptr;
}

QStringList DockController::getTitleList() const {
  QStringList title_list;

  for (const auto& data : item_list_) {
    if (data.isMaximum()) {
      title_list.emplaceFront(data.getTtile());
    }
  }

  return title_list;
}

DockItemData DockController::findDataByItem(QObject* item) {
  DockItemData data{ sender() };
  for (const auto& list_data : item_list_) {
    if (list_data == data) {
      return list_data;
    }
  }

  return DockItemData{};
}

void DockController::onItemMaximumChanged(bool maximum) {
  DockItemData data = findDataByItem(sender());
  if (!data.isVaild()) { return; }

  if (maximum) {
    data.cache_.x = data.getX();
    data.cache_.y = data.getY();
    data.cache_.w = data.getWidth();
    data.cache_.h = data.getHeight();

    data.setX(context_.getX());
    data.setY(context_.getY());
    data.setWidth(context_.getMaxWidth());
    data.setHeight(context_.getMaxHeight());

    return;
  }

  if (data.cache_) {
    data.setX(data.cache_.x);
    data.setY(data.cache_.y);
    data.setWidth(data.cache_.w);
    data.setHeight(data.cache_.h);

    data.cache_.clear();
  }
}
