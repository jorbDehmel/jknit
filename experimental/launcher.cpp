#include <iostream>
int main()
{
#if (defined(_WIN32) || defined(_WIN64))
    std::system("setx /M PATH \"%PATH%%;C:\\WINDOWS\\system32;C:\\WINDOWS;C:\\Program Files\\Python 3.9\"");
    return std::system("python3 ..\\gui.py");
#else
    std::cout << "This launcher is windows-only.\n";
    return 1;
#endif
}
