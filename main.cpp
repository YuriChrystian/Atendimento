#include <chrono>
#include <ctime>
#include <iostream>
#include <list>
#include <locale>


struct People {
    std::string name;
    int priority;
    std::string date;

    bool operator==(const People& other) const {
        return name == other.name && priority == other.priority && date == other.date;
    }
};

std::list<People>::iterator findFirstPeopleByPriority(
    std::list<People> &pending,
    int priority
) {
    auto found = pending.end();

    for (auto it = pending.begin(); it != pending.end(); ++it) {
        if (it->priority == priority) {
            return it;
        }

        if (it->priority < priority) {
            found = it;
        }
    }

    return found;
}

std::string currentDateTime() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm* local_time = std::localtime(&now_time);
    std::stringstream ss;
    ss << std::put_time(local_time, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

void addNewPeople(std::list<People> &pending) {
    system("cls");
    auto *p = new People();
    std::cout << "Nome: ";
    std::cin.ignore();
    std::getline(std::cin, p->name);

    while (p->priority < 1 || p->priority > 3) {
        std::cout << "Prioridade (1-3): ";
        std::cin >> p->priority;
        p->date = currentDateTime();
    }

    if (p->priority == 1) {
        pending.push_back(*p);
        delete p;
        return;
    }

    auto target = findFirstPeopleByPriority(pending, p->priority-1);
    pending.insert(target, *p);

    delete p;
}

void listAllPeoples(std::list<People> &pending) {
    system("cls");
    std::cout << std::endl;

    if (pending.empty()) {
        std::cout << "Lista vazia!" << std::endl;
        system("pause");
        return;
    }

    for (int index = 0; index < pending.size(); ++index) {
        People p = *std::next(pending.begin(), index);

        if (p.priority > 2) {
            std::cout << "(Prioridade ALTA) ";
        } else if (p.priority == 2) {
            std::cout << "(Prioridade MEDIA) ";
        } else {
            std::cout << "(Prioridade BAIXA) ";
        }
        std::cout << p.name << " Deu entrada em " << p.date << std::endl;
    }

    std::cout << std::endl;
    system("pause");
}

void addAttendant(std::string &attendant) {
    system("cls");
    std::cout << "Nome do atendente: ";
    std::cin.ignore();
    std::getline(std::cin,attendant);
}

void callNext(
    std::list<People> &pending,
    std::list<People> &done,
    const std::string &attendant
) {
    system("cls");

    if (attendant.empty()) {
        std::cout << "Nenhum atendente cadastrado!" << std::endl;
        system("pause");
        return;
    }

    if (pending.empty()) {
        std::cout << "Nenhuma pessoa na fila!" << std::endl;
        system("pause");
        return;
    }

    People p = pending.front();
    pending.pop_front();
    p.date = currentDateTime();
    done.push_back(p);

    std::cout << "Atendente: " << attendant << std::endl;
    std::cout << "Chamando proximo: " << p.name << std::endl;

    system("pause");
}

void listAllDonePeoples(std::list<People> &done) {
    system("cls");
    std::cout << std::endl;

    if (done.empty()) {
        std::cout << "Nenhum atendimento realizado!" << std::endl;
        system("pause");
        return;
    }

    for (int index = 0; index < done.size(); ++index) {
        People p = *std::next(done.begin(), index);

        std::cout << index << " - ";
        if (p.priority > 2) {
            std::cout << "(Prioridade ALTA) ";
        } else if (p.priority == 2) {
            std::cout << "(Prioridade MEDIA) ";
        } else {
            std::cout << "(Prioridade BAIXA) ";
        }
        std::cout << p.name << " Atendido em " << p.date << std::endl;
    }

    std::cout << std::endl;
    system("pause");
}

void chancePriority(std::list<People> &pending) {
    system("cls");
    std::cout << std::endl;

    if (pending.empty()) {
        std::cout << "Nenhum atendimento realizado!" << std::endl;
        system("pause");
        return;
    }

    for (int index = 0; index < pending.size(); ++index) {
        People p = *std::next(pending.begin(), index);

        std::cout << index << " - ";
        if (p.priority > 2) {
            std::cout << "(ALTA) ";
        } else if (p.priority == 2) {
            std::cout << "(MEDIA) ";
        } else {
            std::cout << "(BAIXA) ";
        }

        std::cout << p.name << std::endl << std::endl;
    }

    int targetIndex;

    std::cout << "Digite o numero da pessoa que deseja trocar a prioridade " << std::endl;
    std::cout << "Pessoa alvo : ";
    std::cin >> targetIndex;

    if (targetIndex < 0 || targetIndex >= pending.size()) {
        std::cout << "Índice inválido!" << std::endl;
        system("pause");
        return;
    }

    People target = *std::next(pending.begin(), targetIndex);
    int nextPriority = 0;

    std::cout << "Nova prioridade (1-3): ";
    std::cin >> nextPriority;

    pending.remove({target.name, target.priority, target.date});
    target.priority = nextPriority;

    auto insertPos = findFirstPeopleByPriority(pending, target.priority);
    if (insertPos == pending.end()) {
        pending.push_front(target);
    } else {
        pending.insert(insertPos, target);
    }
}

void removeFilteredPeople(std::list<People> &pending, int priority) {
    pending.remove_if([priority](const People &p) {
        return p.priority == priority;
    });
}

int main() {
    auto title = "Totem de atendimento";
    std::list<People> pending;
    std::list<People> done;

    std::string attendant = "Roberto Carlos";

    while (true) {
        system("cls");
        std::cout << title << std::endl;
        std::cout << "1 - Entrar na fila" << std::endl;
        std::cout << "2 - Listar pessoas" << std::endl;
        std::cout << "3 - Trocar atendente" << std::endl;
        std::cout << "4 - Chamar proximo" << std::endl;
        std::cout << "5 - Listar pessoas atendidas" << std::endl;
        std::cout << "6 - Trocar prioridade" << std::endl;
        std::cout << "0 - Sair" << std::endl;
        std::cout << std::endl << "Aguardando valor: ";

        int option;
        std::cin >> option;

        switch (option) {
            case 1: addNewPeople(pending); break;
            case 2: listAllPeoples(pending); break;
            case 3: addAttendant(attendant); break;
            case 4: callNext(pending, done, attendant); break;
            case 5: listAllDonePeoples(done); break;
            case 6: chancePriority(pending); break;
            case 0: return  0;
            default:
                std::cout << "Opção inválida!" << std::endl;
                break;
        }
    }
    return 0;
}
