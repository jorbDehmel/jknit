#include <iostream>
using namespace std;

int main()
{
#if (defined(_WIN32) || defined(_WIN64))
    system("setx /M PATH \"%PATH%%;C:\\WINDOWS\\system32;C:\\WINDOWS;C:\\Program Files\\Python 3.9\"");
    
    if (filesystem::is_directory("C:\\Program Files\\jknit"))
    {
        return system("python3 \"C:\\Program Files\\jknit\\gui.py\"");
    }
    else
    {
        return system("python3 ..\\gui.py");
    }
#else
    cout << "This launcher is windows-only.\n";
    return 1;
#endif
}
