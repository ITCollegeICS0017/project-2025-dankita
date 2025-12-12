

#include <iostream>
#include <vector>

// Infrastructure
#include "interfaces/IDisplay.h"
#include "implementations/ConsoleDisplay.h"

// Configuration
#include "config/Config.h"

// Domain Entities
#include "entities/Client.h"
#include "entities/Service.h"
#include "entities/Consumable.h"
#include "entities/OrderItem.h"
#include "entities/ConsumableUsage.h"
#include "types/Types.h"

// Employees
#include "employees/Employee.h"
#include "employees/Receptionist.h"
#include "employees/Photographer.h"
#include "employees/Administrator.h"

// Orders
#include "orders/Order.h"
#include "orders/ExpressOrder.h"

// Managers
#include "managers/OrderManager.h"
#include "managers/ConsumableManager.h"
#include "managers/ReportManager.h"

// Exceptions - Release 3
#include "exceptions/PhotoStudioExceptions.h"

// Repository - Release 4
#include "repository/OrderRepository.h"
#include "repository/OrderRecord.h"
#include "repository/FileManager.h"

using namespace std;

const string DATA_FILE = "orders.dat";

string getStatusString(OrderStatus status)
{
    switch (status)
    {
    case OrderStatus::PENDING:
        return "PENDING";
    case OrderStatus::IN_PROGRESS:
        return "IN_PROGRESS";
    case OrderStatus::COMPLETED:
        return "COMPLETED";
    case OrderStatus::CANCELLED:
        return "CANCELLED";
    default:
        return "UNKNOWN";
    }
}

/**
 * Main function - Orchestrates the photo studio workflow
 *
 * Main.cpp is now clean and focused on high-level orchestration.
 * Business logic is delegated to manager classes.
 * Dependencies are injected through interfaces (DIP).
 */
int main()
{
    // Setup infrastructure
    ConsoleDisplay display;
    Config config;

    OrderRepository repository;
    FileManager fileManager(DATA_FILE, &display);

    OrderManager orderManager(&display, &config);
    ConsumableManager consumableManager(&display);
    ReportManager reportManager(&display);

    orderManager.setRepository(&repository);
    orderManager.setFileManager(&fileManager);

    display.showLine("=== Photo Studio Operations System - Release 4 ===");
    display.showLine("");

    display.showLine("--- Loading Persistent Data ---");
    orderManager.loadData();

    int loadedCount = orderManager.getLoadedOrderCount();
    display.showLine("");

    try
    {
        // Create employees
        Receptionist receptionist("R001", "Alice");
        Photographer photographer("P001", "Bob");
        Administrator admin("A001", "Charlie");

        display.showLine("--- Employees Initialized ---");
        display.showLine("Receptionist: " + receptionist.getName());
        display.showLine("Photographer: " + photographer.getName());
        display.showLine("Administrator: " + admin.getName());
        display.showLine("");

        Service photoPrinting("S001", "Photo Printing", config.getPhotoPrintingBasePrice(),
                              ServiceType::PHOTO_PRINTING);
        Service filmDeveloping("S002", "Film Developing", config.getFilmDevelopingBasePrice(),
                               ServiceType::FILM_DEVELOPING);

        display.showLine("--- Services Available ---");
        display.showLine("1. " + photoPrinting.getName() + " - $" + to_string(photoPrinting.getBasePrice()));
        display.showLine("2. " + filmDeveloping.getName() + " - $" + to_string(filmDeveloping.getBasePrice()));
        display.showLine("");

        // Setup consumables inventory
        Consumable paper("CON001", "Photo Paper", 1000, "sheets");
        Consumable developer("CON002", "Developer", 50, "liters");

        consumableManager.addConsumable(&paper);
        consumableManager.addConsumable(&developer);

        display.showLine("--- Consumables Inventory ---");
        display.showLine("Photo Paper: " + to_string(paper.getCurrentStock()) + " " + paper.getUnitOfMeasure());
        display.showLine("Developer: " + to_string(developer.getCurrentStock()) + " " + developer.getUnitOfMeasure());
        display.showLine("");

        if (loadedCount > 0)
        {
            display.showLine("=== Working with Loaded Orders ===");
            display.showLine("");

            const vector<Order *> &loadedOrders = orderManager.getAllOrders();

            for (Order *order : loadedOrders)
            {
                display.showLine("Order: " + order->getOrderID() +
                                 " | Client: " + order->getClient()->getSurname() +
                                 " | Status: " + getStatusString(order->getStatus()) +
                                 " | Price: $" + to_string(order->getTotalPrice()) +
                                 " | Paid: " + (order->getIsPaid() ? "Yes" : "No"));

                if (order->getStatus() == OrderStatus::PENDING)
                {
                    display.showLine("  -> Processing pending order...");
                    orderManager.processOrder(order);
                }
                else if (order->getStatus() == OrderStatus::IN_PROGRESS)
                {
                    display.showLine("  -> Completing in-progress order...");
                    orderManager.completeOrder(order);
                    if (!order->getIsPaid() && order->getTotalPrice() > 0)
                    {
                        orderManager.recordPayment(order);
                    }
                }
            }
            display.showLine("");
        }
        else
        {
            display.showLine("=== Creating New Orders (First Run) ===");
            display.showLine("");

            Client *client = orderManager.findOrCreateClient("C001", "Smith");
            display.showLine("Client: " + client->getSurname() + " (ID: " + client->getID() + ")");
            display.showLine("");

            display.showLine("--- Regular Order Workflow ---");

            Order *regularOrder = orderManager.createOrder("O001", client, "2025-09-01 14:00", false);
            receptionist.createOrder(client, regularOrder, &display);

            orderManager.addItemToOrder(regularOrder, "I001", 5, photoPrinting.getBasePrice());

            double regularPrice = regularOrder->calculatePrice();
            display.showLine("Regular order price: $" + to_string(regularPrice));

            orderManager.processOrder(regularOrder);

            ConsumableUsage usage1("U001", "Photo Paper", 50);
            ConsumableUsage usage2("U002", "Developer", 2);
            consumableManager.recordUsage(usage1);
            consumableManager.recordUsage(usage2);

            orderManager.completeOrder(regularOrder);
            orderManager.recordPayment(regularOrder);
            display.showLine("");

            display.showLine("--- Express Order Workflow ---");

            Order *expressOrder = orderManager.createOrder("O002", client, "2025-09-01 18:00", true);
            receptionist.createOrder(client, expressOrder, &display);

            orderManager.addItemToOrder(expressOrder, "I002", 3, filmDeveloping.getBasePrice());

            double expressPrice = expressOrder->calculatePrice();
            display.showLine("Express order price (with 25% surcharge): $" + to_string(expressPrice));

            orderManager.processOrder(expressOrder);
            orderManager.completeOrder(expressOrder);
            orderManager.recordPayment(expressOrder);
            display.showLine("");

            display.showLine("--- Creating Pending Order (for next run demo) ---");

            Client *client2 = orderManager.findOrCreateClient("C002", "Johnson");
            Order *pendingOrder = orderManager.createOrder("O003", client2, "2025-09-02 10:00", false);
            orderManager.addItemToOrder(pendingOrder, "I003", 10, photoPrinting.getBasePrice());
            display.showLine("Created pending order O003 for " + client2->getSurname());
            display.showLine("This order will be loaded and can be processed on the next run!");
            display.showLine("");
        }

        display.showLine("=== End-of-Day Reports ===");
        display.showLine("");

        reportManager.generateDailyRevenueReport(&orderManager);
        display.showLine("");

        reportManager.generateConsumablesUsageReport(&consumableManager);
        display.showLine("");

        display.showLine("=== Repository Status ===");
        display.showLine("");
        display.showLine("Total orders in system: " + to_string(orderManager.getLoadedOrderCount()));
        display.showLine("Repository records: " + to_string(repository.getCount()));
        display.showLine("");

        display.showLine("--- All Orders ---");
        for (Order *order : orderManager.getAllOrders())
        {
            ExpressOrder *express = dynamic_cast<ExpressOrder *>(order);
            display.showLine("  " + order->getOrderID() +
                             " | " + order->getClient()->getSurname() +
                             " | " + (express ? "EXPRESS" : "REGULAR") +
                             " | " + getStatusString(order->getStatus()) +
                             " | $" + to_string(order->getTotalPrice()) +
                             " | " + (order->getIsPaid() ? "PAID" : "UNPAID"));
        }
        display.showLine("");
    }
    catch (const PhotoStudioException &e)
    {
        display.showLine("");
        display.showLine("ERROR: " + string(e.getUserMessage()));
        display.showLine("");

        display.showLine("--- Saving Data Before Exit ---");
        orderManager.saveData();
        return 1;
    }
    catch (const std::exception &e)
    {
        display.showLine("");
        display.showLine("ERROR: Unexpected error occurred.");
        display.showLine("");

        display.showLine("--- Saving Data Before Exit ---");
        orderManager.saveData();
        return 1;
    }

    display.showLine("--- Saving Data Before Exit ---");
    orderManager.saveData();
    display.showLine("");

    display.showLine("=== Photo Studio System Terminated Successfully ===");
    display.showLine("Data has been saved. Run again to see persistence in action!");

    return 0;
}
