

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

using namespace std;

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

    OrderManager orderManager(&display, &config);
    ConsumableManager consumableManager(&display);
    ReportManager reportManager(&display);

    display.showLine("=== Photo Studio Operations System - Release 3 ===");
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

        // Create client
        Client client("C001", "Smith");
        display.showLine("--- Client Registered ---");
        display.showLine("Client: " + client.getSurname() + " (ID: " + client.getID() + ")");
        display.showLine("");

        // Create services (using Config for prices - Open/Closed Principle)
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

        // === WORKFLOW DEMONSTRATION ===

        display.showLine("=== WORKFLOW: Regular Order ===");
        display.showLine("");

        // Receptionist creates a regular order (with validation)
        Order *regularOrder = orderManager.createOrder("O001", &client, "2025-09-01 14:00", false);
        receptionist.createOrder(&client, regularOrder, &display);

        // Add items to order (with validation)
        orderManager.addItemToOrder(regularOrder, "I001", 5, photoPrinting.getBasePrice());

        // Calculate price
        double regularPrice = regularOrder->calculatePrice();
        display.showLine("Regular order price: $" + to_string(regularPrice));
        display.showLine("");

        // Process order workflow (with status validation)
        orderManager.processOrder(regularOrder);

        // Photographer processes the order
        photographer.viewAssignedOrders(&display);

        // Record consumable usage (with stock validation)
        ConsumableUsage usage1("U001", "Photo Paper", 50);
        ConsumableUsage usage2("U002", "Developer", 2);

        vector<ConsumableUsage> usages = {usage1, usage2};
        photographer.submitConsumablesReport(usages, &display);

        // Update inventory (with validation)
        consumableManager.recordUsage(usage1);
        consumableManager.recordUsage(usage2);
        display.showLine("");

        // Complete order (with validation)
        orderManager.completeOrder(regularOrder);
        orderManager.recordPayment(regularOrder);
        display.showLine("");

        // === POLYMORPHISM DEMONSTRATION ===

        display.showLine("=== WORKFLOW: Express Order ===");
        display.showLine("");

        // Create express order
        Order *expressOrder = orderManager.createOrder("O002", &client, "2025-09-01 18:00", true);
        receptionist.createOrder(&client, expressOrder, &display);

        // Add items to express order
        orderManager.addItemToOrder(expressOrder, "I002", 3, filmDeveloping.getBasePrice());

        // Calculate price
        double expressPrice = expressOrder->calculatePrice();
        display.showLine("Express order price (with " +
                         to_string(static_cast<int>(config.getExpressSurchargeRate() * 100)) +
                         "% surcharge): $" + to_string(expressPrice));
        display.showLine("");

        // Process express order
        orderManager.processOrder(expressOrder);
        orderManager.completeOrder(expressOrder);
        orderManager.recordPayment(expressOrder);
        display.showLine("");

        // === ADMINISTRATOR WORKFLOW ===

        display.showLine("=== Administrator Operations ===");
        display.showLine("");

        admin.manageConsumablesStock(&display);
        admin.reviewConsumablesReports(&display);
        display.showLine("");

        // === END-OF-DAY REPORTS ===

        display.showLine("=== End-of-Day Reports ===");
        display.showLine("");

        // Receptionist generates revenue report
        receptionist.generateDailyRevenueReport(&display);
        reportManager.generateDailyRevenueReport(&orderManager);
        display.showLine("");

        // Generate consumables usage report
        reportManager.generateConsumablesUsageReport(&consumableManager);
        display.showLine("");
    }
    catch (const InvalidInputException &e)
    {
        // UI Layer Exception - Invalid user input
        display.showLine("");
        display.showLine("ERROR: Invalid Input");
        display.showLine("Message: " + string(e.getUserMessage()));
        display.showLine("Please check your input and try again.");
        display.showLine("");
        return 1;
    }
    catch (const InvalidDataException &e)
    {
        // Logic Layer Exception - Invalid data
        display.showLine("");
        display.showLine("ERROR: Invalid Data");
        display.showLine("Message: " + string(e.getUserMessage()));
        display.showLine("Please correct the data and try again.");
        display.showLine("");
        return 1;
    }
    catch (const ValidationException &e)
    {
        // Logic Layer Exception - Validation failure
        display.showLine("");
        display.showLine("ERROR: Validation Failed");
        display.showLine("Message: " + string(e.getUserMessage()));
        display.showLine("The operation could not be completed.");
        display.showLine("");
        return 1;
    }
    catch (const BusinessRuleException &e)
    {
        // Logic Layer Exception - Business rule violation
        display.showLine("");
        display.showLine("ERROR: Business Rule Violation");
        display.showLine("Message: " + string(e.getUserMessage()));
        display.showLine("Please follow the business rules.");
        display.showLine("");
        return 1;
    }
    catch (const InsufficientStockException &e)
    {
        // Repository Layer Exception - Insufficient stock
        display.showLine("");
        display.showLine("ERROR: Insufficient Stock");
        display.showLine("Message: " + string(e.getUserMessage()));
        display.showLine("Please check inventory levels.");
        display.showLine("");
        return 1;
    }
    catch (const DataNotFoundException &e)
    {
        // Repository Layer Exception - Data not found
        display.showLine("");
        display.showLine("ERROR: Data Not Found");
        display.showLine("Message: " + string(e.getUserMessage()));
        display.showLine("The requested data could not be located.");
        display.showLine("");
        return 1;
    }
    catch (const DuplicateDataException &e)
    {
        // Repository Layer Exception - Duplicate data
        display.showLine("");
        display.showLine("ERROR: Duplicate Data");
        display.showLine("Message: " + string(e.getUserMessage()));
        display.showLine("This record already exists.");
        display.showLine("");
        return 1;
    }
    catch (const PhotoStudioException &e)
    {
        // Base exception - catch-all for any other custom exceptions
        display.showLine("");
        display.showLine("ERROR: Application Error");
        display.showLine("Message: " + string(e.getUserMessage()));
        display.showLine("Please contact support if this persists.");
        display.showLine("");
        return 1;
    }
    catch (const std::exception &e)
    {
        // Standard exception - unexpected errors
        display.showLine("");
        display.showLine("ERROR: Unexpected Error");
        display.showLine("An unexpected error occurred.");
        display.showLine("Please contact technical support.");
        display.showLine("");
        return 1;
    }

    return 0;
}
