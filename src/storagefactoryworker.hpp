#ifndef STORAGEFACTORYWORKER_HPP
#define STORAGEFACTORYWORKER_HPP

#include <QObject>

#include "xworkertemplate.hpp"
#include "storagedatainterface.hpp"


X_WORKER_DECLARATION_TEMPLATE(StorageFactoryWorker, StorageDataInterface, Q_OBJECT)

#endif //STORAGEFACTORYWORKER_HPP
