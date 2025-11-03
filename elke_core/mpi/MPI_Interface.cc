#include "MPI_Interface.h"
#include "elke_core/output/elk_exceptions.h"

namespace elke
{

MPI_Interface::MPI_Interface(const MPI_Comm communicator)
  : m_communicator(communicator),
    m_rank(MPI_Interface::getRankFromCommunicator(m_communicator)),
    m_num_ranks(MPI_Interface::getNumRanksFromCommunicator(m_communicator))
{
}

MPI_Comm MPI_Interface::communicator() const
{
  elkLogicalErrorIf(not m_mpi_is_initialized,
                    "MPI_Interface used without being initialized.");
  return m_communicator;
}
int MPI_Interface::rank() const
{
  elkLogicalErrorIf(not m_mpi_is_initialized,
                    "MPI_Interface used without being initialized.");
  return m_rank;
}
int MPI_Interface::num_ranks() const
{
  elkLogicalErrorIf(not m_mpi_is_initialized,
                    "MPI_Interface used without being initialized.");
  return m_num_ranks;
}

void MPI_Interface::reinitializeMPI_Interface(int argc,
                                              char** argv,
                                              MPI_Comm communicator)
{
#ifdef MPI_IS_DEFINED
  MPI_Init(&argc, &argv);                    /* starts MPI */
  MPI_Comm_rank(communicator, &m_rank);      /* get cur process id */
  MPI_Comm_size(communicator, &m_num_ranks); /* get num of processes */
#endif
  m_communicator = communicator;

  m_mpi_is_initialized = true;
}

int MPI_Interface::getRankFromCommunicator(MPI_Comm communicator)
{
  int rank = 0;
#ifdef MPI_IS_DEFINED
  MPI_Comm_rank(communicator, &rank);      /* get cur process id */
#endif
  return rank;
}

int MPI_Interface::getNumRanksFromCommunicator(MPI_Comm communicator)
{
  int num_ranks = 1;
#ifdef MPI_IS_DEFINED
  MPI_Comm_size(communicator, &num_ranks);      /* get cur process id */
#endif
  return num_ranks;
}

void MPI_Interface::FinalizeMPI()
{
#ifdef MPI_IS_DEFINED
  MPI_Finalize();
#endif
}

void MPI_Interface::AbortMPI(int error_code)
{
#ifdef MPI_IS_DEFINED
  MPI_Abort(m_communicator, error_code);
#endif
}



} // namespace elke
