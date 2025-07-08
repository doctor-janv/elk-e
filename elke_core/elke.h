#ifndef ELKE_H
#define ELKE_H

namespace elk
{

/**Registers items as a command line interface.*/
void setupGlobalCLI();
/**Parses CLAs and makes sure each one is valid.*/
void parseCommandLine(int argc, char** argv);
/**Given that the list of supplied CLAs is available. This routine responds
 * to specific arguments.*/
void initialCLIResponse();

int main(int argc, char** argv);

} // namespace elk

#endif // ELKE_H
