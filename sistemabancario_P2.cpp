#include <iostream>
#include <iomanip>
#include <string>
#include <limits>

using namespace std;

const int LIMITE_CONTAS = 100;
const int LIMITE_MOVIMENTACOES = 500;

struct Conta {
    int numero_conta = 0;
    int tipo_conta = 0;
    string nome_cliente = "";
    string cpf_cliente = "";
    string tipoConta = "";
    string statusConta = "";
    double saldo = 0.0;
    bool status_conta = false;
};

struct Movimentacao {
    int numero_conta = 0;
    char tipo_movimentacao = '\0';
    double valor = 0.0;
    int conta_relacionada = 0;
};

Conta contas[LIMITE_CONTAS];
Movimentacao movimentacoes[LIMITE_MOVIMENTACOES];
int quantidadeContas = 0;
int quantidadeMovimentacoes = 0;
int opcao = 0;

int buscarConta(int numero);
void ConsultarConta(const Conta& conta);
bool validarCPF(const string& cpf);
bool lerValor(const string& mensagem, double& valor);
void cadastrarConta();
void listarContas();
void consultarContaCadastrada();
void verificarSaldo();
void depositar();
void sacar();
void trasferir();
void extrato();
void AlterarTipoConta();
void alterarStatusConta();

bool validarCPF(const string& cpf) {
    if (cpf.length() != 11) {
        return false;
    }

    for (char caractere : cpf) {
        if (caractere < '0' || caractere > '9') {
            return false;
        }
    }

    bool todosIguais = true;
    for (int i = 1; i < 11; i++) {
        if (cpf[i] != cpf[0]) {
            todosIguais = false;
            break;
        }
    }

    if (todosIguais) {
        return false;
    }

    int soma = 0;
    int peso = 10;

    for (int i = 0; i < 9; i++) {
        soma += (cpf[i] - '0') * peso;
        peso--;
    }

    int resto = soma % 11;
    int digito1;

    if (resto < 2) {
        digito1 = 0;
    } else {
        digito1 = 11 - resto;
    }

    if (digito1 != cpf[9] - '0') {
        return false;
    }

    soma = 0;
    peso = 11;

    for (int i = 0; i < 10; i++) {
        soma += (cpf[i] - '0') * peso;
        peso--;
    }

    resto = soma % 11;
    int digito2;

    if (resto < 2) {
        digito2 = 0;
    } else {
        digito2 = 11 - resto;
    }

    if (digito2 != cpf[10] - '0') {
        return false;
    }

    return true;
}

int buscarConta(int numero) {
    for (int i = 0; i < quantidadeContas; i++) {
        if (contas[i].numero_conta == numero) {
            return i;
        }
    }

    return -1;
}

bool lerValor(const string& mensagem, double& valor) {
    cout << mensagem;

    if (!(cin >> valor)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return false;
    }

    return true;
}

void lerNumeroConta(Conta& conta) {
    while (true) {
        cout << "\nDigite o numero positivo da conta: ";
        cin >> conta.numero_conta;

        if (conta.numero_conta <= 0) {
            cout << "Numero invalido. Tente novamente.\n";
        } else if (buscarConta(conta.numero_conta) != -1) {
            cout << "Esse numero ja esta cadastrado.\n";
        } else {
            break;
        }
    }
}

void lerNomeCliente(Conta& conta) {
    cout << "Digite o nome do cliente: ";
    getline(cin >> ws, conta.nome_cliente);
}

void lerCPF(Conta& conta) {
    do {
        cout << "Digite o CPF do cliente (apenas numeros): ";
        cin >> conta.cpf_cliente;

        if (!validarCPF(conta.cpf_cliente)) {
            cout << "CPF invalido, tente novamente.\n";
        }
    } while (!validarCPF(conta.cpf_cliente));
}

void lerTipoConta(Conta& conta) {
    do {
        cout << "Digite o tipo de conta:\n";
        cout << "(1) Poupanca\n";
        cout << "(2) Corrente\n";
        cout << "Opcao: ";
        cin >> conta.tipo_conta;

        if (conta.tipo_conta != 1 && conta.tipo_conta != 2) {
            cout << "Tipo invalido.\n";
        }
    } while (conta.tipo_conta != 1 && conta.tipo_conta != 2);

    if (conta.tipo_conta == 1) {
        conta.tipoConta = "Poupanca";
    } else {
        conta.tipoConta = "Corrente";
    }
}

void lerSaldo(Conta& conta) {
    do {
        cout << "Digite o saldo inicial (valor igual ou maior que zero): R$";
        cin >> conta.saldo;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            conta.saldo = -1;
        }

        if (conta.saldo < 0) {
            cout << "Saldo invalido.\n";
        }
    } while (conta.saldo < 0);
}

void lerStatusConta(Conta& conta) {
    int status;

    do {
        cout << "Digite o status da conta:\n";
        cout << "(1) Ativa\n";
        cout << "(0) Inativa\n";
        cout << "Opcao: ";
        cin >> status;

        if (status != 0 && status != 1) {
            cout << "Status invalido. Digite 0 ou 1.\n";
        }
    } while (status != 0 && status != 1);

    conta.status_conta = (status == 1);

    if (conta.status_conta) {
        conta.statusConta = "Ativa";
    } else {
        conta.statusConta = "Inativa";
    }
}

void ConsultarConta(const Conta& conta) {
    cout << fixed << setprecision(2);
    cout << "\n====================================\n";
    cout << "Numero da conta: " << conta.numero_conta << '\n';
    cout << "Nome do cliente: " << conta.nome_cliente << '\n';
    cout << "CPF do cliente: " << conta.cpf_cliente << '\n';
    cout << "Tipo de conta: " << conta.tipoConta << '\n';
    cout << "Saldo: R$ " << conta.saldo << '\n';
    cout << "Status da conta: " << conta.statusConta << '\n';
    cout << "====================================\n";
}

void cadastrarConta() {
    if (quantidadeContas >= LIMITE_CONTAS) {
        cout << "Limite de 100 contas atingido.\n";
        return;
    }

    Conta conta;
    lerNumeroConta(conta);
    lerNomeCliente(conta);
    lerCPF(conta);
    lerTipoConta(conta);
    lerSaldo(conta);
    lerStatusConta(conta);
    ConsultarConta(conta);

    string confirmacao;
    cout << "Os dados estao corretos? (s/n): ";
    cin >> confirmacao;

    while (confirmacao == "n" || confirmacao == "N") {
        int dado;

        cout << "Qual dado deseja alterar?\n";
        cout << "1. Numero\n";
        cout << "2. Nome\n";
        cout << "3. CPF\n";
        cout << "4. Tipo\n";
        cout << "5. Saldo\n";
        cout << "6. Status\n";
        cout << "Opcao: ";
        cin >> dado;

        switch (dado) {
            case 1:
                lerNumeroConta(conta);
                break;
            case 2:
                lerNomeCliente(conta);
                break;
            case 3:
                lerCPF(conta);
                break;
            case 4:
                lerTipoConta(conta);
                break;
            case 5:
                lerSaldo(conta);
                break;
            case 6:
                lerStatusConta(conta);
                break;
            default:
                cout << "Opcao invalida.\n";
                continue;
        }

        ConsultarConta(conta);
        cout << "Os dados estao corretos agora? (s/n): ";
        cin >> confirmacao;
    }

    contas[quantidadeContas] = conta;
    quantidadeContas++;
    cout << "Conta cadastrada com sucesso!\n";
}

void listarContas() {
    if (quantidadeContas == 0) {
        cout << "Nenhuma conta cadastrada.\n";
        return;
    }

    cout << "\n--- CONTAS CADASTRADAS ---\n";
    for (int i = 0; i < quantidadeContas; i++) {
        ConsultarConta(contas[i]);
    }
}

void consultarContaCadastrada() {
    int numero;
    cout << "Digite o numero da conta: ";
    cin >> numero;

    int indice = buscarConta(numero);
    if (indice == -1) {
        cout << "Conta nao encontrada.\n";
        return;
    }

    ConsultarConta(contas[indice]);
}

void verificarSaldo() {
    int numero;
    cout << "Digite o numero da conta: ";
    cin >> numero;

    int indice = buscarConta(numero);
    if (indice == -1) {
        cout << "Conta nao encontrada.\n";
        return;
    }

    if (!contas[indice].status_conta) {
        cout << "Conta inativa.\n";
        return;
    }

    cout << fixed << setprecision(2);
    cout << "Saldo: R$ " << contas[indice].saldo << '\n';
}

void depositar() {
    int numero;
    double valor;

    cout << "Digite o numero da conta: ";
    cin >> numero;

    int indice = buscarConta(numero);
    if (indice == -1) {
        cout << "Conta nao encontrada.\n";
        return;
    }

    if (!contas[indice].status_conta) {
        cout << "Conta inativa.\n";
        return;
    }

    if (quantidadeMovimentacoes >= LIMITE_MOVIMENTACOES) {
        cout << "Limite de movimentacoes atingido.\n";
        return;
    }

    if (!lerValor("Valor do deposito: R$ ", valor) || valor <= 0) {
        cout << "Valor invalido.\n";
        return;
    }

    contas[indice].saldo += valor;

    movimentacoes[quantidadeMovimentacoes].numero_conta = numero;
    movimentacoes[quantidadeMovimentacoes].tipo_movimentacao = 'D';
    movimentacoes[quantidadeMovimentacoes].valor = valor;
    movimentacoes[quantidadeMovimentacoes].conta_relacionada = 0;
    quantidadeMovimentacoes++;

    cout << fixed << setprecision(2);
    cout << "Deposito realizado. Saldo atual: R$ " << contas[indice].saldo << '\n';
}

void sacar() {
    int numero;
    double valor;

    cout << "Digite o numero da conta: ";
    cin >> numero;

    int indice = buscarConta(numero);
    if (indice == -1) {
        cout << "Conta nao encontrada.\n";
        return;
    }

    if (!contas[indice].status_conta) {
        cout << "Conta inativa.\n";
        return;
    }

    if (quantidadeMovimentacoes >= LIMITE_MOVIMENTACOES) {
        cout << "Limite de movimentacoes atingido.\n";
        return;
    }

    if (!lerValor("Valor do saque: R$ ", valor) || valor <= 0) {
        cout << "Valor invalido.\n";
        return;
    }

    if (valor > contas[indice].saldo) {
        cout << "Saldo insuficiente!\n";
        return;
    }

    contas[indice].saldo -= valor;

    movimentacoes[quantidadeMovimentacoes].numero_conta = numero;
    movimentacoes[quantidadeMovimentacoes].tipo_movimentacao = 'S';
    movimentacoes[quantidadeMovimentacoes].valor = valor;
    movimentacoes[quantidadeMovimentacoes].conta_relacionada = 0;
    quantidadeMovimentacoes++;

    cout << fixed << setprecision(2);
    cout << "Saque realizado. Saldo atual: R$ " << contas[indice].saldo << '\n';
}

void trasferir() {
    int numeroOrigem;
    int numeroDestino;
    double valor;

    cout << "Digite o numero da conta de origem: ";
    cin >> numeroOrigem;

    int indiceOrigem = buscarConta(numeroOrigem);
    if (indiceOrigem == -1) {
        cout << "Conta de origem nao encontrada.\n";
        return;
    }

    if (!contas[indiceOrigem].status_conta) {
        cout << "Conta de origem inativa.\n";
        return;
    }

    cout << "Digite o numero da conta de destino: ";
    cin >> numeroDestino;

    if (numeroDestino == numeroOrigem) {
        cout << "Nao e possivel transferir para a mesma conta.\n";
        return;
    }

    int indiceDestino = buscarConta(numeroDestino);
    if (indiceDestino == -1) {
        cout << "Conta de destino nao encontrada.\n";
        return;
    }

    if (!contas[indiceDestino].status_conta) {
        cout << "Conta de destino inativa.\n";
        return;
    }

    if (!lerValor("Valor da transferencia: R$ ", valor) || valor <= 0) {
        cout << "Valor invalido.\n";
        return;
    }

    if (valor > contas[indiceOrigem].saldo) {
        cout << "Saldo insuficiente!\n";
        return;
    }

    if (LIMITE_MOVIMENTACOES - quantidadeMovimentacoes < 2) {
        cout << "Nao ha espaco para registrar as duas movimentacoes.\n";
        cout << "Os saldos foram mantidos.\n";
        return;
    }

    contas[indiceOrigem].saldo -= valor;
    contas[indiceDestino].saldo += valor;

    movimentacoes[quantidadeMovimentacoes].numero_conta = numeroOrigem;
    movimentacoes[quantidadeMovimentacoes].tipo_movimentacao = 'S';
    movimentacoes[quantidadeMovimentacoes].valor = valor;
    movimentacoes[quantidadeMovimentacoes].conta_relacionada = numeroDestino;
    quantidadeMovimentacoes++;

    movimentacoes[quantidadeMovimentacoes].numero_conta = numeroDestino;
    movimentacoes[quantidadeMovimentacoes].tipo_movimentacao = 'D';
    movimentacoes[quantidadeMovimentacoes].valor = valor;
    movimentacoes[quantidadeMovimentacoes].conta_relacionada = numeroOrigem;
    quantidadeMovimentacoes++;

    cout << fixed << setprecision(2);
    cout << "Transferencia realizada!\n";
    cout << "Saldo da conta de origem: R$ " << contas[indiceOrigem].saldo << '\n';
}

void extrato() {
    int numero;
    cout << "Digite o numero da conta: ";
    cin >> numero;

    int indice = buscarConta(numero);
    if (indice == -1) {
        cout << "Conta nao encontrada.\n";
        return;
    }

    cout << fixed << setprecision(2);
    cout << "\nExtrato da conta " << numero << " - ";
    cout << contas[indice].nome_cliente << '\n';

    bool encontrou = false;
    for (int i = 0; i < quantidadeMovimentacoes; i++) {
        const Movimentacao& movimentacao = movimentacoes[i];

        if (movimentacao.numero_conta != numero) {
            continue;
        }

        encontrou = true;
        if (movimentacao.tipo_movimentacao == 'D' &&
            movimentacao.conta_relacionada != 0) {
            cout << "Transferencia recebida de ";
            cout << movimentacao.conta_relacionada << ": + R$ ";
            cout << movimentacao.valor << '\n';
        } else if (movimentacao.tipo_movimentacao == 'D') {
            cout << "Deposito: + R$ " << movimentacao.valor << '\n';
        } else if (movimentacao.tipo_movimentacao == 'S' &&
                   movimentacao.conta_relacionada != 0) {
            cout << "Transferencia enviada para ";
            cout << movimentacao.conta_relacionada << ": - R$ ";
            cout << movimentacao.valor << '\n';
        } else {
            cout << "Saque: - R$ " << movimentacao.valor << '\n';
        }
    }

    if (!encontrou) {
        cout << "Nenhuma movimentacao registrada.\n";
    }

    cout << "Saldo atual: R$ " << contas[indice].saldo << '\n';
}

void AlterarTipoConta() {
    int numero;
    cout << "Digite o numero da conta: ";
    cin >> numero;

    int indice = buscarConta(numero);
    if (indice == -1) {
        cout << "Conta nao encontrada.\n";
        return;
    }

    if (!contas[indice].status_conta) {
        cout << "Conta inativa. Ative-a primeiro.\n";
        return;
    }

    cout << "Tipo atual: " << contas[indice].tipoConta << '\n';
    lerTipoConta(contas[indice]);
    cout << "Tipo alterado para: " << contas[indice].tipoConta << '\n';
}

void alterarStatusConta() {
    int numero;
    cout << "Digite o numero da conta: ";
    cin >> numero;

    int indice = buscarConta(numero);
    if (indice == -1) {
        cout << "Conta nao encontrada.\n";
        return;
    }

    contas[indice].status_conta = !contas[indice].status_conta;
    if (contas[indice].status_conta) {
        contas[indice].statusConta = "Ativa";
    } else {
        contas[indice].statusConta = "Inativa";
    }

    cout << "Conta " << contas[indice].statusConta << " com sucesso.\n";
}

int main() {
    do {
        cout << "\n===============================================\n";
        cout << "              BANCO SOLUMK\n";
        cout << "===============================================\n";
        cout << "1. Cadastrar conta\n";
        cout << "2. Listar contas\n";
        cout << "3. Consultar conta\n";
        cout << "4. Verificar saldo\n";
        cout << "5. Depositar\n";
        cout << "6. Sacar\n";
        cout << "7. Transferir\n";
        cout << "8. Exibir extrato\n";
        cout << "9. Alterar tipo de conta\n";
        cout << "10. Ativar/desativar conta\n";
        cout << "11. Sair\n";
        cout << "===============================================\n";
        cout << "Selecione uma opcao: ";
        cin >> opcao;

        switch (opcao) {
            case 1:
                cadastrarConta();
                break;
            case 2:
                listarContas();
                break;
            case 3:
                consultarContaCadastrada();
                break;
            case 4:
                verificarSaldo();
                break;
            case 5:
                depositar();
                break;
            case 6:
                sacar();
                break;
            case 7:
                trasferir();
                break;
            case 8:
                extrato();
                break;
            case 9:
                AlterarTipoConta();
                break;
            case 10:
                alterarStatusConta();
                break;
            case 11:
                cout << "\nSistema encerrado.\n";
                break;
            default:
                cout << "Opcao invalida.\n";
                break;
        }
    } while (opcao != 11);

    return 0;
}
