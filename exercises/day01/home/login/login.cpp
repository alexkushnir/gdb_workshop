#include <print>
#include <string>

bool validPassword = true;
bool invalidPassword = false;

// TARGET FUNCTION: Runs constantly for valid users.
// We ONLY want to break here if an error was just shown.
void LoadUserProfile(const std::string& username)
{
    std::println("Loading private profile data for {}...", username);
    // Bug: If we are here after a failed login, we have a security breach!
}

// TRIGGER FUNCTION: Runs rarely, only when a password is wrong.
void ShowErrorMessage(const std::string& error)
{
    std::println("[ERROR] {}", error);
}

bool ValidatePassword(const std::string& username, bool isValidPassword)
{
    // Simulates password validation
    return isValidPassword;
}

void ProcessLogin(const std::string& username, bool isValidPassword)
{
    bool isValid = ValidatePassword(username, isValidPassword);

    if (!isValid)
    {
        ShowErrorMessage("Invalid password for " + username);
    }

    // THE BUG: We forgot to return out of the function after the error!
    // So this runs for everyone, even failed logins.
    LoadUserProfile(username);
}

int main()
{
    std::println("--- Server Started ---");

    // Simulate 100 normal, successful logins
    for (int i = 1; i <= 100; ++i)
    {
        ProcessLogin("User" + std::to_string(i), validPassword);
    }

    // Simulate ONE user typing the wrong password
    ProcessLogin("HackerBob", invalidPassword);

    return 0;
}
