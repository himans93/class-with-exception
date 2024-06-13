#include <iostream>
#include <memory>
#include <optional>
#include <string>

// Define an interface for database resource
class IDatabaseResource {
public:
    virtual ~IDatabaseResource() = default;
    virtual void executeQuery(const std::string& query) = 0;
};

//Implement the Concrete DatabaseResource Class:
class DatabaseResource : public IDatabaseResource {
public:
    static std::optional<std::unique_ptr<DatabaseResource>> create(const std::string& connectionString) {
        if (connectionString.empty()) {
            return std::nullopt; // Return empty optional to indicate failure
        }
        return std::make_unique<DatabaseResource>(connectionString);
    }

    void executeQuery(const std::string& query) override {
        std::cout << "Executing query: " << query << std::endl;
    }

private:
    // Private constructor to enforce creation through the static method
    DatabaseResource(const std::string& connectionString) {
        std::cout << "Database connected using: " << connectionString << std::endl;
    }
};
