#ifndef TRANSACTIONGUARD_H
#define TRANSACTIONGUARD_H
#include <QSqlDatabase>
#include <QSqlDriver>
#include "LoggerStream.h"
class TransactionGuard {
 public:
  TransactionGuard(QSqlDatabase& db): db_(db), active_(false) {
    if (db_.driver()->hasFeature(QSqlDriver::Transactions)) {
      db_.rollback();
      active_ = db_.transaction();
    }
  }
  bool ok() const { return active_; }
  bool commit() {
    if (!active_) return false;
    committed_ = db_.commit();
    active_ = false;
    if (committed_) minfo() << "Transaction commited successfully!";
    return committed_;
  }
  ~TransactionGuard() {
    if (active_) db_.rollback();
  }
 private:
  QSqlDatabase& db_;
  bool active_;
  bool committed_ = false;
};

#endif  // TRANSACTIONGUARD_H
