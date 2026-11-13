#include <algorithm>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using namespace std;

namespace {
string lireTexte(const string& invite) {
    string valeur;
    cout << invite;
    getline(cin, valeur);
    return valeur;
}

int lireEntier(const string& invite, int minimum = 0) {
    while (true) {
        string ligne = lireTexte(invite);
        stringstream flux(ligne);
        int valeur;
        char reste;
        if ((flux >> valeur) && !(flux >> reste) && valeur >= minimum) return valeur;
        cout << "Valeur invalide. Entrez un entier superieur ou egal a " << minimum << ".\n";
    }
}

double lireMontant(const string& invite) {
    while (true) {
        string ligne = lireTexte(invite);
        replace(ligne.begin(), ligne.end(), ',', '.');
        stringstream flux(ligne);
        double valeur;
        char reste;
        if ((flux >> valeur) && !(flux >> reste) && valeur >= 0.0) return valeur;
        cout << "Montant invalide. Entrez un nombre positif ou nul.\n";
    }
}
}

class Passenger {
    int id_;
    string name_;
    string phone_;
    string email_;
public:
    Passenger(int id, string name, string phone, string email)
        : id_(id), name_(move(name)), phone_(move(phone)), email_(move(email)) {}
    int id() const { return id_; }
    const string& name() const { return name_; }
    void display() const {
        cout << "Passager #" << id_ << " | " << name_ << " | Tel: " << phone_
             << " | Email: " << email_ << '\n';
    }
};

class Flight {
    int id_;
    string origin_;
    string destination_;
    string departure_;
    int capacity_;
    double price_;
public:
    Flight(int id, string origin, string destination, string departure, int capacity, double price)
        : id_(id), origin_(move(origin)), destination_(move(destination)),
          departure_(move(departure)), capacity_(capacity), price_(price) {}
    int id() const { return id_; }
    int capacity() const { return capacity_; }
    double price() const { return price_; }
    const string& origin() const { return origin_; }
    const string& destination() const { return destination_; }
    void display(int placesRestantes) const {
        cout << "Vol #" << id_ << " | " << origin_ << " -> " << destination_
             << " | Depart: " << departure_ << " | Prix: " << fixed << setprecision(2)
             << price_ << " | Places disponibles: " << placesRestantes << '\n';
    }
};

enum class ReservationStatus { CONFIRMED, CANCELLED };
enum class PaymentStatus { UNPAID, PAID, REFUNDED };

class Reservation {
    int id_;
    int passengerId_;
    int flightId_;
    ReservationStatus status_;
    PaymentStatus payment_;
public:
    Reservation(int id, int passengerId, int flightId)
        : id_(id), passengerId_(passengerId), flightId_(flightId),
          status_(ReservationStatus::CONFIRMED), payment_(PaymentStatus::UNPAID) {}
    int id() const { return id_; }
    int passengerId() const { return passengerId_; }
    int flightId() const { return flightId_; }
    bool active() const { return status_ == ReservationStatus::CONFIRMED; }
    bool paid() const { return payment_ == PaymentStatus::PAID; }
    void pay() { payment_ = PaymentStatus::PAID; }
    void cancel() {
        status_ = ReservationStatus::CANCELLED;
        if (payment_ == PaymentStatus::PAID) payment_ = PaymentStatus::REFUNDED;
    }
    string statusText() const {
        return status_ == ReservationStatus::CONFIRMED ? "Confirmee" : "Annulee";
    }
    string paymentText() const {
        switch (payment_) {
            case PaymentStatus::PAID: return "Payee";
            case PaymentStatus::REFUNDED: return "Remboursee";
            default: return "En attente";
        }
    }
};

class Agency {
    map<int, Passenger> passengers_;
    map<int, Flight> flights_;
    map<int, Reservation> reservations_;
    int nextPassengerId_ = 1;
    int nextFlightId_ = 1;
    int nextReservationId_ = 1;

    int activeReservationsForFlight(int flightId) const {
        int total = 0;
        for (const auto& item : reservations_)
            if (item.second.flightId() == flightId && item.second.active()) ++total;
        return total;
    }
    bool reservationExists(int passengerId, int flightId) const {
        for (const auto& item : reservations_)
            if (item.second.passengerId() == passengerId && item.second.flightId() == flightId)
                return true; // La regle interdit une seconde reservation, meme apres annulation.
        return false;
    }
public:
    void registerPassenger() {
        string name = lireTexte("Nom complet: ");
        string phone = lireTexte("Telephone: ");
        string email = lireTexte("Email: ");
        if (name.empty()) { cout << "Le nom est obligatoire.\n"; return; }
        int id = nextPassengerId_++;
        passengers_.emplace(id, Passenger(id, name, phone, email));
        cout << "Passager enregistre avec l'identifiant " << id << ".\n";
    }
    void addFlight() {
        string origin = lireTexte("Ville de depart: ");
        string destination = lireTexte("Destination: ");
        string departure = lireTexte("Date et heure (ex. 20/12/2026 08:30): ");
        int capacity = lireEntier("Capacite: ", 1);
        double price = lireMontant("Prix du billet: ");
        if (origin.empty() || destination.empty() || departure.empty() || origin == destination) {
            cout << "Informations de vol invalides.\n"; return;
        }
        int id = nextFlightId_++;
        flights_.emplace(id, Flight(id, origin, destination, departure, capacity, price));
        cout << "Vol ajoute avec l'identifiant " << id << ".\n";
    }
    void showAvailableFlights() const {
        bool found = false;
        for (const auto& item : flights_) {
            int remaining = item.second.capacity() - activeReservationsForFlight(item.first);
            if (remaining > 0) { item.second.display(remaining); found = true; }
        }
        if (!found) cout << "Aucun vol disponible.\n";
    }
    void searchFlights() const {
        string origin = lireTexte("Depart: ");
        string destination = lireTexte("Destination: ");
        bool found = false;
        for (const auto& item : flights_) {
            const Flight& flight = item.second;
            if (flight.origin() == origin && flight.destination() == destination) {
                int remaining = flight.capacity() - activeReservationsForFlight(item.first);
                flight.display(remaining); found = true;
            }
        }
        if (!found) cout << "Aucun vol correspondant.\n";
    }
    void makeReservation() {
        if (passengers_.empty()) { cout << "Enregistrez d'abord un passager.\n"; return; }
        if (flights_.empty()) { cout << "Aucun vol n'est enregistre.\n"; return; }
        int passengerId = lireEntier("Identifiant du passager: ", 1);
        int flightId = lireEntier("Identifiant du vol: ", 1);
        if (!passengers_.count(passengerId)) { cout << "Passager introuvable.\n"; return; }
        auto flight = flights_.find(flightId);
        if (flight == flights_.end()) { cout << "Vol introuvable.\n"; return; }
        if (reservationExists(passengerId, flightId)) {
            cout << "Ce passager a deja une reservation pour ce vol.\n"; return;
        }
        if (activeReservationsForFlight(flightId) >= flight->second.capacity()) {
            cout << "Le vol a atteint sa capacite.\n"; return;
        }
        int id = nextReservationId_++;
        reservations_.emplace(id, Reservation(id, passengerId, flightId));
        cout << "Reservation confirmee sous le numero " << id << ".\n";
    }
    void recordPayment() {
        int id = lireEntier("Numero de reservation: ", 1);
        auto it = reservations_.find(id);
        if (it == reservations_.end()) { cout << "Reservation introuvable.\n"; return; }
        if (!it->second.active()) { cout << "Une reservation annulee ne peut pas etre payee.\n"; return; }
        if (it->second.paid()) { cout << "Cette reservation est deja payee.\n"; return; }
        cout << "Montant a regler: " << fixed << setprecision(2) << flights_.at(it->second.flightId()).price() << '\n';
        string validation = lireTexte("Confirmer le paiement (o/n): ");
        if (validation == "o" || validation == "O") { it->second.pay(); cout << "Paiement enregistre.\n"; }
        else cout << "Paiement non enregistre.\n";
    }
    void cancelReservation() {
        int id = lireEntier("Numero de reservation: ", 1);
        auto it = reservations_.find(id);
        if (it == reservations_.end()) { cout << "Reservation introuvable.\n"; return; }
        if (!it->second.active()) { cout << "Cette reservation est deja annulee.\n"; return; }
        it->second.cancel();
        cout << (it->second.paymentText() == "Remboursee"
                 ? "Reservation annulee; remboursement enregistre.\n"
                 : "Reservation annulee.\n");
    }
    void consultReservations() const {
        if (reservations_.empty()) { cout << "Aucune reservation.\n"; return; }
        for (const auto& item : reservations_) displayReservation(item.second);
    }
    void displayReservationInfo() const {
        int id = lireEntier("Numero de reservation: ", 1);
        auto it = reservations_.find(id);
        if (it == reservations_.end()) { cout << "Reservation introuvable.\n"; return; }
        displayReservation(it->second);
    }
private:
    void displayReservation(const Reservation& reservation) const {
        const Passenger& passenger = passengers_.at(reservation.passengerId());
        const Flight& flight = flights_.at(reservation.flightId());
        cout << "Reservation #" << reservation.id() << " | ";
        passenger.display();
        flight.display(flight.capacity() - activeReservationsForFlight(flight.id()));
        cout << "Statut: " << reservation.statusText() << " | Paiement: "
             << reservation.paymentText() << "\n\n";
    }
};

int main() {
    Agency agency;
    while (true) {
        cout << "\n=== AGENCE DE VOYAGE ===\n"
             << "1. Enregistrer un passager\n2. Ajouter un vol\n"
             << "3. Afficher les vols disponibles\n4. Rechercher un vol\n"
             << "5. Effectuer une reservation\n6. Enregistrer un paiement\n"
             << "7. Annuler une reservation\n8. Consulter les reservations\n"
             << "9. Afficher les informations d'une reservation\n10. Quitter\n";
        int choix = lireEntier("Votre choix: ", 1);
        switch (choix) {
            case 1: agency.registerPassenger(); break;
            case 2: agency.addFlight(); break;
            case 3: agency.showAvailableFlights(); break;
            case 4: agency.searchFlights(); break;
            case 5: agency.makeReservation(); break;
            case 6: agency.recordPayment(); break;
            case 7: agency.cancelReservation(); break;
            case 8: agency.consultReservations(); break;
            case 9: agency.displayReservationInfo(); break;
            case 10: cout << "Au revoir.\n"; return 0;
            default: cout << "Choix invalide.\n";
        }
    }
}
