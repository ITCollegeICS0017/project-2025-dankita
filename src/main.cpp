#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <memory>

using namespace std;

enum class OrderStatus
{
    PENDING,
    IN_PROGRESS,
    COMPLETED,
    CANCELLED
};

enum class ReportType
{
    DAILY_REVENUE,
    CONSUMABLES_USAGE
};

enum class ServiceType
{
    PHOTO_PRINTING,
    FILM_DEVELOPING
};

struct OrderItem
{
    string itemID;
    int quantity;
    double unitPrice;

    OrderItem(string id, int qty, double price)
    {
        this->itemID = id;
        this->quantity = qty;
        this->unitPrice = price;
    }

    double getSubtotal()
    {
        return quantity * unitPrice;
    }
};

struct ConsumableUsage
{
    string usageID;
    string consumableName;
    int quantityUsed;
    string usageDate;

    ConsumableUsage(string id, string name, int qty, string date)
    {
        this->usageID = id;
        this->consumableName = name;
        this->quantityUsed = qty;
        this->usageDate = date;
    }
};

class Service;
class Client;
class Order;
class Consumable;
class Report;
class Receptionist;
class Photographer;
class Administrator;

class Service
{
private:
    string serviceID;
    string name;
    double basePrice;
    ServiceType type;

public:
    Service(const string &id, const string &n, double price, ServiceType t)
    {
        this->serviceID = id;
        this->name = n;
        this->basePrice = price;
        this->type = t;
    }

    double getBasePrice() { return basePrice; }
    string getName() { return name; }
    ServiceType getType() { return type; }
};

class Consumable
{
private:
    string consumableID;
    string name;
    int currentStock;
    string unitOfMeasure;

public:
    Consumable(const string &id, const string &n, int stock, const string &unit)
    {
        this->consumableID = id;
        this->name = n;
        this->currentStock = stock;
        this->unitOfMeasure = unit;
    }

    void updateStock(int quantity)
    {
        currentStock += quantity;
        cout << "Stock updated for " << name << ": " << currentStock << " " << unitOfMeasure << endl;
    }

    int getCurrentStock() { return currentStock; }
    string getName() { return name; }
};

class Report
{
private:
    string reportID;
    ReportType reportType;
    string generationDate;
    string content;

public:
    Report(const string &id, ReportType type, const string &date, const string &cont)
    {
        this->reportID = id;
        this->reportType = type;
        this->generationDate = date;
        this->content = cont;
    }

    string getContent() { return content; }
    ReportType getReportType() { return reportType; }
};

class Client
{
private:
    string clientID;
    string surname;

public:
    Client(const string &id, const string &sname)
    {
        this->clientID = id;
        this->surname = sname;
    }

    string getSurname() { return surname; }
    string getID() { return clientID; }
};

class Employee
{
protected:
    string employeeID;
    string name;

public:
    Employee(const string &id, const string &n)
    {
        this->employeeID = id;
        this->name = n;
    }

    string getName() { return name; }
    string getID() { return employeeID; }
};

class Receptionist : public Employee
{
public:
    Receptionist(string id, string n) : Employee(id, n)
    {
    }

    void createOrder(Client *client, Order *order)
    {
        cout << "Receptionist " << name << " creating order for client " << client->getSurname() << endl;
    }

    void generateDailyRevenueReport()
    {
        cout << "Generating daily revenue report..." << endl;
    }
};

class Photographer : public Employee
{
public:
    Photographer(string id, string n) : Employee(id, n)
    {
    }

    void viewAssignedOrders()
    {
        cout << "Photographer " << name << " viewing assigned orders..." << endl;
    }

    void submitConsumablesReport(vector<ConsumableUsage> items)
    {
        cout << "Photographer " << name << " submitting consumables report..." << endl;
    }
};

class Administrator : public Employee
{
public:
    Administrator(string id, string n) : Employee(id, n)
    {
    }

    void manageConsumablesStock()
    {
        cout << "Administrator " << name << " managing consumables stock..." << endl;
    }

    void reviewConsumablesReports()
    {
        cout << "Administrator " << name << " reviewing consumables reports..." << endl;
    }
};

class Order
{
private:
    string orderID;
    string orderDate;
    string completionTime;
    OrderStatus status;
    double totalPrice;
    bool isPaid;
    Client *client;
    vector<OrderItem *> items = {};

public:
    Order(const string &id, const string &date, const string &cTime, Client *c)
    {
        this->orderID = id;
        this->orderDate = date;
        this->completionTime = cTime;
        this->status = OrderStatus::PENDING;
        this->totalPrice = 0.0;
        this->isPaid = false;
        this->client = c;
    }

    virtual double calculatePrice()
    {
        cout << "Calculating price for order " << orderID << endl;
        return totalPrice;
    }

    void updateStatus(OrderStatus newStatus)
    {
        status = newStatus;
        cout << "Order " << orderID << " status updated to ";
        switch (status)
        {
        case OrderStatus::PENDING:
            cout << "PENDING";
            break;
        case OrderStatus::IN_PROGRESS:
            cout << "IN_PROGRESS";
            break;
        case OrderStatus::COMPLETED:
            cout << "COMPLETED";
            break;
        case OrderStatus::CANCELLED:
            cout << "CANCELLED";
            break;
        }
        cout << endl;
    }

    void recordPayment()
    {
        isPaid = true;
        cout << "Payment recorded for order " << orderID << endl;
    }

    void addItem(OrderItem *item)
    {
        items.push_back(item);
        totalPrice += item->getSubtotal();
    }
};

class ExpressOrder : public Order
{
private:
    static constexpr double expressSurcharge = 0.25;

public:
    ExpressOrder(string id, string date, string cTime, Client *c)
        : Order(id, date, cTime, c)
    {
    }

    double calculatePrice() override
    {
        double basePrice = Order::calculatePrice();
        double expressPrice = basePrice * (1 + expressSurcharge);
        cout << "Express order price (with 25% surcharge): " << expressPrice << endl;
        return expressPrice;
    }
};

int main()
{

    // Create employees
    Receptionist receptionist("R001", "Alice");
    Photographer photographer("P001", "Bob");
    Administrator admin("A001", "Charlie");

    // Create client
    Client client("C001", "Smith");

    // Create services
    Service photoPrinting("S001", "Photo Printing", 15.99, ServiceType::PHOTO_PRINTING);
    Service filmDeveloping("S002", "Film Developing", 25.50, ServiceType::FILM_DEVELOPING);

    // Create consumables
    Consumable paper("CON001", "Photo Paper", 1000, "sheets");
    Consumable developer("CON002", "Developer", 50, "liters");

    // Create order
    Order *order = new Order("O001", "2025-09-22", "2025-09-23 14:00", &client);

    // Add items to order
    OrderItem *item1 = new OrderItem("I001", 5, photoPrinting.getBasePrice());
    order->addItem(item1);

    // Process order
    receptionist.createOrder(&client, order);
    order->calculatePrice();
    order->updateStatus(OrderStatus::IN_PROGRESS);
    order->updateStatus(OrderStatus::COMPLETED);
    order->recordPayment();

    // Photographer reports
    photographer.viewAssignedOrders();
    vector<ConsumableUsage> consumables = {
        ConsumableUsage("U001", "Photo Paper", 50, "2025-09-22"),
        ConsumableUsage("U002", "Developer", 2, "2025-09-22")};
    photographer.submitConsumablesReport(consumables);

    // Update consumables stock
    paper.updateStock(-50);
    developer.updateStock(-2);

    // Admin manages
    admin.manageConsumablesStock();
    admin.reviewConsumablesReports();

    // Generate reports
    receptionist.generateDailyRevenueReport();

    return 0;
}
