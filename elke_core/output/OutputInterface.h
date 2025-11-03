#ifndef ELK_E_OUTPUTINTERFACE_H
#define ELK_E_OUTPUTINTERFACE_H

#include "elke_core/output/Logger.h"

namespace elke
{

class OutputInterface
{
  std::shared_ptr<Logger> m_logger_ptr;

public:
  explicit OutputInterface(int rank);

  Logger& getLogger() const;

  std::shared_ptr<Logger> getLoggerPtr() const;
};

} // namespace elke

#endif // ELK_E_OUTPUTINTERFACE_H
