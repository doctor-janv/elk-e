#ifndef ELK_E_MPI_INTERFACE_H
#define ELK_E_MPI_INTERFACE_H

#ifndef MPI_IS_DEFINED
typedef int MPI_Comm;
inline MPI_Comm MPI_COMM_WORLD = 0;
#else
#include <mpi.h>
#endif

namespace elke
{

/**An encapsulation of all MPI-related members and methods.*/
class MPI_Interface
{
  friend class FrameworkCore;

  MPI_Comm m_communicator = 0;       ///< MPI-communicator
  int m_rank = 0;                    ///< Rank of the current process
  int m_num_ranks = 1;               ///< Number of ranks on the communicator
  bool m_mpi_is_initialized = false; ///< Is Initialized? flag

  MPI_Interface() = default; // Only used by core module

public:
  /**Communicator based constructor.*/
  explicit MPI_Interface(MPI_Comm communicator);

  /**Returns the communicator of this context.*/
  MPI_Comm communicator() const;
  /**Returns the rank of the context.*/
  int rank() const;
  /**Returns the number of ranks on the communicator.*/
  int num_ranks() const;


private:
  void FinalizeMPI();
  void AbortMPI(int error_code);
  void reinitializeMPI_Interface(int argc, char** argv, MPI_Comm communicator);
  static int getRankFromCommunicator(MPI_Comm communicator);
  static int getNumRanksFromCommunicator(MPI_Comm communicator);
};

} // namespace elke

#endif // ELK_E_MPI_INTERFACE_H
