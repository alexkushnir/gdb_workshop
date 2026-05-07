#include <iostream>
#include <vector>
#include <cstring>
#include <string>

struct Record 
{
    int m_id;
    char m_username[12];
    int m_accessLevel;
};

void SecureClearName(Record& r) 
{
    // BUG: Off-by-one error in memset. 
    // The username buffer is 12 bytes, but we clear 13.
    // This overwrites the first byte of 'access_level'.
    std::memset(r.m_username, 0, 13); 
}

void ProcessDatabase(std::vector<Record>& db) 
{
    for (auto& r : db) 
    {
        if (r.m_id == 2) 
        {
            SecureClearName(r);
        }
    }
}

int main() 
{
    std::vector<Record> db = 
    {
        {1, "admin", 100},
        {2, "guest", 1},
        {3, "operator", 50}
    };

    std::cout << "Starting database processing..." << std::endl;
    std::cout << "Guest (ID 2) access level: " << db[1].m_accessLevel << std::endl;

    ProcessDatabase(db);

    // BREAKPOINT HERE:
    // Notice that Guest's access level is no longer 1.
    // 1. Run to this point.
    // 2. Check the value: print db[1].m_accessLevel
    // 3. Set a watchpoint: watch db[1].m_accessLevel
    // 4. Use 'reverse-continue' to find where the value was changed.
    
    if (db[1].m_accessLevel != 1) 
    {
        std::cout << "BUG DETECTED: Guest access level corrupted! Current value: " 
                  << db[1].m_accessLevel << std::endl;
    } else 
    {
        std::cout << "Database processed successfully." << std::endl;
    }

    return 0;
}
