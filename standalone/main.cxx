/**
 * The main function for the standalone program. It expects a
 * filename as argument, tries to parse the configuration file
 * and starts the main loop, if successful
 */

// Standard includes
#include <iostream>
#include <string>

// OpenVideo includes
#include <openvideo/Manager.h>

// Using namespaces
using namespace std;
using namespace openvideo;

int main(int argc, char ** argv)
{
  if (argc != 2) {
    cout << "Usage : " << argv[0] <<  " configfile" << endl;
    return 1;
  }

  cout << "**************************************" << endl;
  cout << "**** OpenVideo -standalone- v1.0 *****" << endl;
  cout << "**************************************" << endl;
  
  Manager * manager = Manager::getInstance();
  string ovConfig = argv[1];
  manager->parseConfiguration(ovConfig);
  cout << "Parsing complete." << endl;
    
  manager->run();

  return 0;
}
