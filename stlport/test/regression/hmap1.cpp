#define MAIN
#ifdef MAIN 
#define hmap1_test main
#endif
// STLport regression testsuite component.
// To compile as a separate example, please #define MAIN.


#pragma warning (disable: 4786)
#include <iostream>
#include <hash_map>
#include <rope>
#include <stdlib.h>
#include <stdio.h>

#if !defined (STLPORT) || defined(__STL_USE_NAMESPACES)
using namespace std;
#endif

int hmap1_test(int, char**)
{
  cout<<"Results of hmap1_test:"<<endl;
  typedef hash_map<char, crope, hash<char>, equal_to<char> > maptype;
  maptype m;
  // Store mappings between roman numerals and decimals.
  m['l'] = "50";
  m['x'] = "20"; // Deliberate mistake.
  m['v'] = "5";
  m['i'] = "1";
  cout << "m['x'] = " << m['x'] << endl;
  m['x'] = "10"; // Correct mistake.
  cout << "m['x'] = " << m['x'] << endl;
  cout << "m['z'] = " << m['z'] << endl; // Note default value is added.
  cout << "m.count('z') = " << m.count('z') << endl;
  pair<maptype::iterator, bool> p =
      m.insert(pair<const char, crope>('c', crope("100")));
  if(p.second)
    cout << "First insertion successful" << endl;
  p = m.insert(pair<const char, crope>('c', crope("100")));
  if(p.second)
    cout << "Second insertion successful" << endl;
  else
    cout << "Existing pair " <<(*(p.first)).first
         << " -> " <<(*(p.first)).second << endl;
  getchar();
  return 0;
}
