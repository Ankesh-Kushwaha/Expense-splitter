#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>

using namespace std;
class User
{
public:
  int id;
  string name;

  User() {}

  User(int id, string name)
  {
    this->id = id;
    this->name = name;
  }
};

class Expense
{
public:
  int id;
  string description;
  double amount;
  int paidBy;
  vector<int> participants;

  Expense(int id, string desc, double amt, int payer, vector<int> parts)
  {
    this->id = id;
    description = desc;
    amount = amt;
    paidBy = payer;
    participants = parts;
  }
};

class ExpenseManager
{
private:
  unordered_map<int, User> users;
  vector<Expense> expenses;

  // balances[A][B] = amount A owes B
  unordered_map<int, unordered_map<int, double>> balances;

public:
  // 1️⃣ Add User
  void addUser()
  {
    int id;
    string name;

    cout << "Enter User ID: ";
    cin >> id;

    if (users.count(id))
    {
      cout << "User already exists!\n";
      return;
    }

    cout << "Enter Name: ";
    cin >> name;

    users.emplace(id, User(id, name));
    cout << "User added successfully!\n";
  }

  // 2️⃣ Add Expense
  void addExpense()
  {
    if (users.empty())
    {
      cout << "No users available. Add users first.\n";
      return;
    }

    int id, paidBy, n;
    string desc;
    double amount;

    cout << "Enter Expense ID: ";
    cin >> id;

    cout << "Enter Description: ";
    cin >> desc;

    cout << "Enter Total Amount: ";
    cin >> amount;

    cout << "Paid By (User ID): ";
    cin >> paidBy;

    if (!users.count(paidBy))
    {
      cout << "Payer does not exist!\n";
      return;
    }

    cout << "Number of Participants: ";
    cin >> n;

    vector<int> participants;
    cout << "Enter Participant User IDs:\n";

    for (int i = 0; i < n; i++)
    {
      int uid;
      cin >> uid;

      if (!users.count(uid))
      {
        cout << "User " << uid << " not found. Ignored.\n";
        continue;
      }

      // avoid duplicates
      bool exists = false;
      for (int x : participants)
        if (x == uid)
          exists = true;

      if (!exists)
        participants.push_back(uid);
    }

    if (participants.empty())
    {
      cout << "No valid participants.\n";
      return;
    }

    Expense exp(id, desc, amount, paidBy, participants);
    expenses.push_back(exp);

    double share = amount / participants.size();

    for (int userId : participants)
    {
      if (userId == paidBy)
        continue;

      balances[userId][paidBy] += share;
      balances[paidBy][userId] -= share;
    }

    cout << "Expense added successfully!\n";
  }

  // 3️⃣ View Expenses
  void viewExpenses()
  {
    cout << "\n--- All Expenses ---\n";

    if (expenses.empty())
    {
      cout << "No expenses.\n";
      return;
    }

    for (auto &e : expenses)
    {
      cout << "ID: " << e.id
           << ", Desc: " << e.description
           << ", Amount: " << e.amount
           << ", Paid By: " << users[e.paidBy].name
           << endl;
    }
  }

  // 4️⃣ Show Balances
  void showBalances()
  {
    cout << "\n--- Balances ---\n";

    bool empty = true;

    for (auto &u1 : balances)
    {
      for (auto &u2 : u1.second)
      {
        int from = u1.first;
        int to = u2.first;
        double amt = u2.second;

        if (amt > 0.01)
        {
          empty = false;
          cout << users[from].name
               << " owes "
               << users[to].name
               << " Rs." << amt << endl;
        }
      }
    }

    if (empty)
      cout << "No balances.\n";
  }
};

int main()
{

  ExpenseManager manager;
  int choice;

  while (true)
  {

    cout << "\n===== Expense Splitter =====\n";
    cout << "1. Add User\n";
    cout << "2. Add Expense\n";
    cout << "3. View Expenses\n";
    cout << "4. Show Balances\n";
    cout << "5. Exit\n";
    cout << "Enter choice: ";

    cin >> choice;

    switch (choice)
    {
    case 1:
      manager.addUser();
      break;

    case 2:
      manager.addExpense();
      break;

    case 3:
      manager.viewExpenses();
      break;

    case 4:
      manager.showBalances();
      break;

    case 5:
      cout << "Exiting...\n";
      return 0;

    default:
      cout << "Invalid choice!\n";
    }
  }
}