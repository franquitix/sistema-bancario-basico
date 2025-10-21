#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>

using namespace std;

//  Clase Cuenta
class Cuenta {
private:
    int numeroCuenta;
    double saldo;

public:
    // Constructor
    Cuenta(int num, double sal) : numeroCuenta(num), saldo(sal) {}

    // Getters
    int getNumeroCuenta() const {
        return numeroCuenta;
    }

    double getSaldo() const {
        return saldo;
    }

    // Métodos principales 
    void depositar(double monto) {
        if (monto > 0) {
            saldo += monto;
            cout << "Depósito exitoso. Saldo actual: $" << fixed << setprecision(2) << saldo << endl;
        } else {
            cout << "Error: El monto de depósito debe ser positivo." << endl;
        }
    }

    bool retirar(double monto) {
        if (monto > 0) {
            if (monto <= saldo) {
                saldo -= monto;
                cout << "Retiro exitoso. Saldo actual: $" << fixed << setprecision(2) << saldo << endl;
                return true;
            } else {
                cout << "Error: Saldo insuficiente." << endl;
                return false;
            }
        } else {
            cout << "Error: El monto de retiro debe ser positivo." << endl;
            return false;
        }
    }

    // Mostrar información de la cuenta
    void mostrarInfo() const {
        cout << "   - Número de Cuenta: " << numeroCuenta << endl;
        cout << "   - Saldo: $" << fixed << setprecision(2) << saldo << endl;
    }
};

// Clase Cliente
class Cliente {
private:
    string nombre;
    int idCliente;
    vector<Cuenta> cuentas; // Un cliente puede tener varias cuentas

public:
    // Constructor
    Cliente(int id, string nom) : idCliente(id), nombre(nom) {}

    // Getters
    int getIdCliente() const {
        return idCliente;
    }

    string getNombre() const {
        return nombre;
    }

    const vector<Cuenta>& getCuentas() const {
        return cuentas;
    }

    // Métodos para cuentas
    void agregarCuenta(int numCuenta, double saldoInicial) {
        cuentas.emplace_back(numCuenta, saldoInicial);
        cout << "\n>>> Cuenta N° " << numCuenta << " agregada a " << nombre << " con éxito." << endl;
    }

    Cuenta* buscarCuenta(int numCuenta) {
        for (size_t i = 0; i < cuentas.size(); ++i) {
            if (cuentas[i].getNumeroCuenta() == numCuenta) {
                return &cuentas[i]; // Retorna un puntero a la cuenta encontrada
            }
        }
        return nullptr; // Si no la encuentra
    }

    void mostrarInfo() const {
        cout << "\n--- Información del Cliente ---" << endl;
        cout << "ID: " << idCliente << ", Nombre: " << nombre << endl;
        cout << "Cuentas Asociadas:" << endl;
        if (cuentas.empty()) {
            cout << "   (No tiene cuentas asociadas)." << endl;
            return;
        }
        for (const auto& cuenta : cuentas) {
            cuenta.mostrarInfo();
        }
        cout << "-----------------------------" << endl;
    }
};

// Gestion de los clientes
class Banco {
private:
    vector<Cliente> clientes;
    int proximoIdCliente = 1001; // ID inicial para clientes
    int proximoNumCuenta = 50001; // Número de cuenta inicial

public:
    // Métodos para clientes
    void agregarCliente(const string& nombre) {
        clientes.emplace_back(proximoIdCliente++, nombre);
        cout << "\n[!] Cliente " << nombre << " registrado con ID: " << proximoIdCliente - 1 << endl;
    }

    Cliente* buscarCliente(int idCliente) {
        for (size_t i = 0; i < clientes.size(); ++i) {
            if (clientes[i].getIdCliente() == idCliente) {
                return &clientes[i];
            }
        }
        return nullptr;
    }

    void crearCuentaParaCliente(int idCliente, double saldoInicial) {
        Cliente* cliente = buscarCliente(idCliente);
        if (cliente) {
            cliente->agregarCuenta(proximoNumCuenta++, saldoInicial);
        } else {
            cout << "\nError: Cliente con ID " << idCliente << " no encontrado." << endl;
        }
    }

    // Métodos de Transacción 
    void realizarTransaccion(int idCliente, int numCuenta, double monto, bool esDeposito) {
        Cliente* cliente = buscarCliente(idCliente);
        if (!cliente) {
            cout << "Error: Cliente no encontrado." << endl;
            return;
        }

        Cuenta* cuenta = cliente->buscarCuenta(numCuenta);
        if (!cuenta) {
            cout << "Error: Cuenta N° " << numCuenta << " no encontrada para el cliente " << cliente->getNombre() << "." << endl;
            return;
        }

        if (esDeposito) {
            cuenta->depositar(monto);
        } else {
            cuenta->retirar(monto);
        }
    }

    void mostrarTodosLosClientes() const {
        if (clientes.empty()) {
            cout << "\n(El banco no tiene clientes registrados)." << endl;
            return;
        }
        cout << "\n=== LISTA DE TODOS LOS CLIENTES ===" << endl;
        for (const auto& cliente : clientes) {
            cliente.mostrarInfo();
        }
        cout << "===================================" << endl;
    }
};

//  Función Principal y Menú
void mostrarMenu() {
    cout << "\n\n=== SISTEMA BANCARIO BÁSICO ===" << endl;
    cout << "1. Registrar Nuevo Cliente" << endl;
    cout << "2. Crear Nueva Cuenta" << endl;
    cout << "3. Realizar Depósito" << endl;
    cout << "4. Realizar Retiro" << endl;
    cout << "5. Consultar Información de Cliente" << endl;
    cout << "6. Mostrar Todos los Clientes" << endl;
    cout << "7. Salir" << endl;
    cout << "Seleccione una opción: ";
}

int main() {
    Banco banco;
    int opcion;
    int id, numCuenta;
    string nombre;
    double monto, saldoInicial;
    Cliente* cliente;

    // cliente de ejemplo para probar
    banco.agregarCliente("Juan Perez");
    banco.crearCuentaParaCliente(1001, 500.00);

    do {
        mostrarMenu();
        if (!(cin >> opcion)) {
            cin.clear(); // Limpia el error
            cin.ignore(10000, '\n'); // Descarta la entrada
            opcion = 0;
            continue;
        }

        switch (opcion) {
            case 1: // Registrar Nuevo Cliente
                cout << "Ingrese el nombre del nuevo cliente: ";
                cin.ignore(); // Limpiar el buffer
                getline(cin, nombre);
                banco.agregarCliente(nombre);
                break;

            case 2: // Crear Nueva Cuenta
                cout << "Ingrese el ID del cliente: ";
                cin >> id;
                cout << "Ingrese el saldo inicial: $";
                cin >> saldoInicial;
                banco.crearCuentaParaCliente(id, saldoInicial);
                break;

            case 3: //  Depósito
                cout << "Ingrese el ID del cliente: ";
                cin >> id;
                cout << "Ingrese el número de cuenta: ";
                cin >> numCuenta;
                cout << "Ingrese el monto a depositar: $";
                cin >> monto;
                banco.realizarTransaccion(id, numCuenta, monto, true); // true = depósito
                break;

            case 4: //  Retiro
                cout << "Ingrese el ID del cliente: ";
                cin >> id;
                cout << "Ingrese el número de cuenta: ";
                cin >> numCuenta;
                cout << "Ingrese el monto a retirar: $";
                cin >> monto;
                banco.realizarTransaccion(id, numCuenta, monto, false); // false = retiro
                break;

            case 5: // Consultar Info de Cliente
                cout << "Ingrese el ID del cliente a consultar: ";
                cin >> id;
                cliente = banco.buscarCliente(id);
                if (cliente) {
                    cliente->mostrarInfo();
                } else {
                    cout << "\nError: Cliente con ID " << id << " no encontrado." << endl;
                }
                break;

            case 6: // Mostrar Todos los Clientes
                banco.mostrarTodosLosClientes();
                break;

            case 7: // Salir
                cout << "\nGracias por usar el Sistema Bancario Básico. ¡Adiós!" << endl;
                break;

            default:
                cout << "\nOpción inválida. Intente de nuevo." << endl;
                break;
        }
    } while (opcion != 7);

    return 0;
}