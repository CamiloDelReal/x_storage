#ifndef STORAGEFACTORYWORKER_HPP
#define STORAGEFACTORYWORKER_HPP

#include <QObject>

#include "workertemplate.hpp"
#include "storagedatainterface.hpp"

WORKER_DECLARATION_TEMPLATE(StorageFactoryWorker, StorageDataInterface, Q_OBJECT)

#endif //STORAGEFACTORYWORKER_HPP
