#include <iostream>
#include <string>
#include <cstring>
#include <iomanip>
#include <ctime>
#include <fstream>
#include <limits>
#include <cerrno>

using namespace std;

// Struct untuk Film

void MenuUtama();
void TulisKeFile(const string &username, const string &securityQuestion, const string &securityAnswer);
void Masuk(int attempt);
void DaftarKataSandi(const string &username);
void Daftar();
void KeluarProgram();
void PulihkanKataSandi();
void BersihkanCin();
void AturUlangKataSandi(const string &username);
void SimpanKataSandiBaru(const string &username, const string &newPassword);
void PengaturanAkun(const string &username);

const int TABLE_SIZE = 10;
const int MAX_BIOSKOP = 100; // Maksimal jumlah bioskop
const int INF = 1e9; // Representasi infinity

int jumlahBioskop = 0;
string namaBioskop[MAX_BIOSKOP];
int graph[MAX_BIOSKOP][MAX_BIOSKOP];

int total_seat_select = 0;
int total_to_pay;
int y, x, seat_icon3 = 254;
char seat[4][10];
string data;

int pilihan;
bool cinfail;
int konfirmasi;
string username, password, password2, dateOfBirth, namaLengkap, kotaAsal;

struct Film
{
    string kode;
    string nama;
    string genre;
    string rating;
    Film *next;
};
// Struct untuk Bioskop
struct Bioskop
{
    string nama;
    string alamat;
    int jumlahStudio;
    float hargaTiket;
    Bioskop *next;
};
// Struct untuk Pesanan
struct Pesanan
{
    int id;
    string user;
    Film *film;
    string jam;
    Bioskop *bioskop;
    int studio;
    string bangku;
    float harga;
    string pembayaran;
    string waktuPesan; // Menambahkan waktu pesan
    Pesanan *next;
};

struct Friend {
    string name;
    Friend* next;

    Friend(const string& name) : name(name), next(nullptr) {}
};

struct PesananNode
{
    Pesanan *pesanan;
    PesananNode *next;
};
// Struct untuk User
struct User {
    string username;
    string nama;
    string password;
    string ttl; // tanggal lahir
    string alamat;
    string kotaAsal;
    Friend* friendList;
    User *next;

    // Konstruktor, destructor, dan lain-lain
};

struct TreeNode {
    int data;
    TreeNode* left;
    TreeNode* right;

    TreeNode(int value) : data(value), left(nullptr), right(nullptr) {}
};


struct PesananStackNode
{
    Pesanan *pesanan;
    PesananStackNode *next;
};
//MANAGE AKUN USER
bool InputValid(const string &input)
{
    return !input.empty() && input.length() >= 4;
}

bool UsernameAda(const string &username)
{
    ifstream infile("pengguna.txt");
    string line, storedUsername;
    while (getline(infile, line))
    {
        stringstream ss(line);
        getline(ss, storedUsername, '|');
        if (storedUsername == username)
        {
            return true;
        }
    }
    return false;
}

void TulisKeFile(const string &username, const string &securityQuestion, const string &securityAnswer)
{
    // Menggunakan file pengguna.txt yang sudah ada untuk menyimpan data pengguna
    ofstream writefile("pengguna.txt", ios::app); // Membuka file dengan mode append
    if (writefile.is_open())
    {
        // Menulis data ke file pengguna.txt
        writefile << "user|" << username << "|" << password << "|" << securityQuestion << "|" << securityAnswer << "|" << dateOfBirth << "|" << namaLengkap << "|" << kotaAsal << endl;
        writefile.close();
        cout << "Akun berhasil dibuat.\nTekan Enter untuk kembali ke menu utama...";
        cin.get();
        MenuUtama();
    }
    else
    {
        cout << "Gagal membuka file.\n";
    }
}

bool VerifikasiMasuk(const string &username, const string &password)
{
    ifstream infile("pengguna.txt");
    string line, storedUsername, storedPassword;
    while (getline(infile, line))
    {
        stringstream ss(line);
        getline(ss, storedUsername, '|');
        getline(ss, storedUsername, '|');
        getline(ss, storedPassword, '|');
        if (storedUsername == username && storedPassword == password)
        {
            return true;
        }
    }
    return false;
}

void BersihkanCin()
{
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void DaftarKataSandi(const string &username)
{
    cout << "Silakan masukkan kata sandi: ";
    getline(cin, password);
    cout << "Silakan masukkan ulang kata sandi Anda: ";
    getline(cin, password2);

    if (password == password2 && InputValid(password))
    {
        const string securityQuestions[] = {
            "Apa nama hewan peliharaan pertama Anda?",
            "Di kota mana Anda lahir?",
            "Apa makanan favorit Anda?",
            "Apa nama sekolah dasar yang Anda hadiri?",
            "Di kota mana orang tua Anda bertemu?",
            "Apa judul film favorit Anda?",
            "Apa nama jalan tempat Anda tinggal saat berusia 10 tahun?",
            "Apa pekerjaan impian Anda saat kecil?",
            "Siapa nama sahabat Anda saat masa kecil?",
            "Apa merek mobil pertama Anda?"};
        int numQuestions = sizeof(securityQuestions) / sizeof(string);
        int questionIndex;

        cout << "Pilih pertanyaan keamanan dari daftar berikut dengan memasukkan nomor:\n";
        for (int i = 0; i < numQuestions; i++)
        {
            cout << "[" << i + 1 << "] " << securityQuestions[i] << endl;
        }
        cin >> questionIndex;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (questionIndex < 1 || questionIndex > numQuestions)
        {
            cout << "Pilihan tidak valid. Silakan coba lagi.\n";
            DaftarKataSandi(username);
            return;
        }

        string securityQuestion = securityQuestions[questionIndex - 1];
        string securityAnswer;
        cout << "Masukkan jawaban keamanan: ";
        getline(cin, securityAnswer);

        TulisKeFile(username, securityQuestion, securityAnswer);
        KeluarProgram();
    }
    else
    {
        if (!InputValid(password))
        {
            cout << "Input tidak valid. Kata sandi harus setidaknya 4 karakter.\n";
        }
        else
        {
            cout << "Kata sandi tidak cocok. Coba lagi.\n";
        }
        cout << "Tekan Enter untuk coba lagi...";
        cin.get();
        system("cls");
        DaftarKataSandi(username);
    }
}

void Daftar()
{
    cout << "DAFTAR AKUN\nSilakan masukkan nama pengguna Anda: ";
    getline(cin, username);

    if (UsernameAda(username))
    {
        cout << "Nama pengguna sudah digunakan. Silakan coba nama lain.\nTekan Enter untuk coba lagi...";
        cin.get();
        system("cls");
        Daftar();
        return;
    }

    int konfirmasi;
    cout << "Apakah Anda ingin menggunakan username " << username << "? \n[1] Iya \n[2] Tidak\n > ";
    cin >> konfirmasi;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (konfirmasi == 1)
    {
        cout << "Masukkan nama lengkap Anda: ";
        getline(cin, namaLengkap);
        cout << "Masukkan kota asal Anda: ";
        getline(cin, kotaAsal);
        cout << "Masukkan tanggal lahir Anda (DD/MM/YYYY): ";
        getline(cin, dateOfBirth);
        DaftarKataSandi(username);
    }
    else if (konfirmasi == 2)
    {
        cout << "Silakan masukkan nama pengguna yang lain.\nTekan Enter untuk melanjutkan...";
        cin.get();
        system("cls");
        Daftar();
    }
    else
    {
        cout << "Input tidak valid. Silakan masukkan '1' untuk Iya atau '2' untuk Tidak.\nTekan Enter untuk coba lagi...";
        cin.get();
        system("cls");
        Daftar();
    }
}

void KeluarProgram()
{
    ::exit(0);
}

void PulihkanKataSandi()
{
    string searchUsername, userAnswer;
    cout << "Masukkan nama pengguna Anda untuk memulihkan kata sandi: ";
    cin >> searchUsername;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    ifstream infile("pengguna.txt");
    string line, userType, storedUsername, storedPassword, securityQuestion, correctAnswer, storedDateOfBirth;
    bool found = false;
    bool correctSecurityAnswer = false;
    while (getline(infile, line))
    {
        stringstream ss(line);
        getline(ss, userType, '|');
        getline(ss, storedUsername, '|');
        getline(ss, storedPassword, '|');
        getline(ss, securityQuestion, '|');
        getline(ss, correctAnswer, '|');
        getline(ss, storedDateOfBirth, '|');

        if (storedUsername == searchUsername)
        {
            found = true;
            cout << "Untuk verifikasi, jawab pertanyaan keamanan ini: \n"
                 << securityQuestion << endl;
            getline(cin, userAnswer);
            if (userAnswer == correctAnswer)
            {
                cout << "Verifikasi berhasil. Silakan atur ulang kata sandi Anda.\n";
                AturUlangKataSandi(storedUsername);
                correctSecurityAnswer = true;
                break;
            }
            else
            {
                cout << "Jawaban keamanan salah.\n";
                break;
            }
        }
    }
    if (!found || !correctSecurityAnswer)
    {
        cout << "Nama pengguna tidak ditemukan atau jawaban keamanan salah. Silakan coba lagi.\nTekan enter untuk kembali ke menu utama.\n";
        cin.get();
        MenuUtama();
    }
}

void AturUlangKataSandi(const string &username)
{
    cout << "Masukkan kata sandi baru: ";
    string newPassword;
    getline(cin, newPassword);
    SimpanKataSandiBaru(username, newPassword);
    cout << "Kata sandi telah diatur ulang. Tekan enter untuk kembali ke menu utama.\n";
    cin.get();
    MenuUtama();
}

void SimpanKataSandiBaru(const string &username, const string &newPassword)
{
    ifstream infile("pengguna.txt");
    stringstream tempStream;
    string line, userType, storedUsername, storedPassword, securityQuestion, correctAnswer, storedDateOfBirth;

    while (getline(infile, line))
    {
        stringstream ss(line);
        getline(ss, userType, '|');
        getline(ss, storedUsername, '|');
        getline(ss, storedPassword, '|');
        getline(ss, securityQuestion, '|');
        getline(ss, correctAnswer, '|');
        getline(ss, storedDateOfBirth, '|');

        if (storedUsername == username)
        {
            tempStream << userType << '|' << storedUsername << '|' << newPassword << '|' << securityQuestion << '|' << correctAnswer << '|' << storedDateOfBirth << '\n';
        }
        else
        {
            tempStream << line << '\n';
        }
    }
    infile.close();

    ofstream outfile("pengguna.txt");
    outfile << tempStream.str();
    outfile.close();
}

void updateUserData(User& user)
{
    // Membuka file dengan mode read untuk mencari dan mengganti data pengguna
    ifstream infile("pengguna.txt");
    ofstream tempFile("temp.txt");
    string line;

    if (!infile.is_open() || !tempFile.is_open())
    {
        cout << "Gagal membuka file.\n";
        return;
    }

    while (getline(infile, line))
    {
        stringstream ss(line);
        string userType, storedUsername, storedPassword, storedTtl, storedAlamat;
        getline(ss, userType, '|');
        getline(ss, storedUsername, '|');
        getline(ss, storedPassword, '|');
        getline(ss, storedTtl, '|');
        getline(ss, storedAlamat, '|');

        if (storedUsername == user.nama)
        {
            // Menulis data pengguna yang diperbarui ke file sementara
            tempFile << userType << '|' << user.nama << '|' << user.password << '|' << user.ttl << '|' << user.alamat << '\n';
        }
        else
        {
            // Menulis data yang tidak diubah ke file sementara
            tempFile << line << '\n';
        }
    }

    // Menutup file
    infile.close();
    tempFile.close();

    // Mengganti file pengguna lama dengan yang baru
    remove("pengguna.txt");
    rename("temp.txt", "pengguna.txt");

    cout << "Data pengguna berhasil diperbarui.\n";
}

void ubahPassword(string username)
{
    string passwordBaru;
    cout << "Masukkan password baru: ";
    BersihkanCin();
    getline(cin, passwordBaru);

    ifstream infile("pengguna.txt");
    stringstream tempStream;
    string line, userType, storedUsername, storedPassword, securityQuestion, correctAnswer, storedDateOfBirth;

    bool found = false;
    while (getline(infile, line))
    {
        stringstream ss(line);
        getline(ss, userType, '|');
        getline(ss, storedUsername, '|');
        getline(ss, storedPassword, '|');
        getline(ss, securityQuestion, '|');
        getline(ss, correctAnswer, '|');
        getline(ss, storedDateOfBirth, '|');

        if (storedUsername == username)
        {
            tempStream << userType << '|' << storedUsername << '|' << passwordBaru << '|' << securityQuestion << '|' << correctAnswer << '|' << storedDateOfBirth << '\n';
            found = true;
        }
        else
        {
            tempStream << line << '\n';
        }
    }
    infile.close();

    if (found)
    {
        ofstream outfile("pengguna.txt");
        outfile << tempStream.str();
        outfile.close();
        cout << "Password berhasil diubah.\n";
    }
    else
    {
        cout << "Pengguna tidak ditemukan.\n";
    }
}

void ubahDataDiri(string username)

{
    ifstream infile("pengguna.txt");
    if (!infile.is_open())
    {
        cout << "Gagal membuka file pengguna.txt\n";
        return;
    }

    string namaLengkapBaru, kotaAsalBaru, tanggalLahirBaru;
    stringstream tempStream;
    string line, userType, storedUsername, storedPassword, securityQuestion, correctAnswer, storedDateOfBirth, namaLengkap, kotaAsal;

    bool found = false;
    while (getline(infile, line))
    {
        stringstream ss(line);
        getline(ss, userType, '|');
        getline(ss, storedUsername, '|');
        getline(ss, storedPassword, '|');
        getline(ss, securityQuestion, '|');
        getline(ss, correctAnswer, '|');
        getline(ss, storedDateOfBirth, '|');
        getline(ss, namaLengkap, '|');
        getline(ss, kotaAsal, '|');

        if (storedUsername == username)
        {
            cout << "Nama Lengkap: " << namaLengkap << endl;
            cout << "Username: " << storedUsername << endl;
            cout << "Kota Asal: " << kotaAsal << endl;
            // Mengubah format TTL dari "DD/MM/YYYY" menjadi "Tanggal-Bulan-Tahun"
            if (storedDateOfBirth.length() >= 10)
            {
                string tanggal = storedDateOfBirth.substr(0, 2);
                string bulan = storedDateOfBirth.substr(3, 2);
                string tahun = storedDateOfBirth.substr(6, 4);
                cout << "TTL: " << tanggal << "-" << bulan << "-" << tahun << " (Tanggal-Bulan-Tahun)" << endl;
            }
            else
            {
                cout << "TTL: Data tidak lengkap" << endl;
            }
            int pilihan;
            cout << "Pilih data yang ingin diubah:\n";
            cout << "1. Nama Lengkap\n";
            cout << "2. Kota Asal\n";
            cout << "3. Tanggal Lahir\n";
            cout << "Masukkan pilihan Anda (1-3): ";
            cin >> pilihan;
            BersihkanCin();

            if (pilihan == 1)
            {
                cout << "Masukkan nama lengkap baru: ";
                getline(cin, namaLengkapBaru);
            }
            else if (pilihan == 2)
            {
                cout << "Masukkan kota asal baru: ";
                getline(cin, kotaAsalBaru);
            }
            else if (pilihan == 3)
            {
                cout << "Masukkan tanggal lahir baru (DD/MM/YYYY): ";
                getline(cin, tanggalLahirBaru);
            }

            tempStream << userType << '|' << storedUsername << '|' << storedPassword << '|' << securityQuestion << '|' << correctAnswer << '|' << (pilihan == 3 ? (tanggalLahirBaru.empty() ? storedDateOfBirth : tanggalLahirBaru) : storedDateOfBirth) << '|' << (pilihan == 1 ? (namaLengkapBaru.empty() ? namaLengkap : namaLengkapBaru) : namaLengkap) << '|' << (pilihan == 2 ? (kotaAsalBaru.empty() ? kotaAsal : kotaAsalBaru) : kotaAsal) << '\n';
            found = true;
        }
        else
        {
            tempStream << line << '\n';
        }
    }
    infile.close();

    if (found)
    {
        ofstream outfile("pengguna.txt");
        outfile << tempStream.str();
        outfile.close();
        cout << "Data diri berhasil diubah.\n";
    }
    else
    {
        cout << "Pengguna tidak ditemukan.\n";
    }
    PengaturanAkun(username);
}

void PengaturanAkun(const string &username)
{
    system("cls");
    ifstream infile("pengguna.txt");
    string line, userType, storedUsername, storedPassword, securityQuestion, correctAnswer, storedDateOfBirth, namaLengkap, kotaAsal;
    bool found = false;

    while (getline(infile, line))
    {
        stringstream ss(line);
        getline(ss, userType, '|');
        getline(ss, storedUsername, '|');
        getline(ss, storedPassword, '|');
        getline(ss, securityQuestion, '|');
        getline(ss, correctAnswer, '|');
        getline(ss, storedDateOfBirth, '|');
        getline(ss, namaLengkap, '|');
        getline(ss, kotaAsal, '|');

        if (storedUsername == username)
        {
            found = true;
            cout << "Nama Lengkap: " << namaLengkap << endl;
            cout << "Username: " << storedUsername << endl;
            cout << "Kota Asal: " << kotaAsal << endl;
            // Mengubah format TTL dari "DD/MM/YYYY" menjadi "Tanggal-Bulan-Tahun"
            if (storedDateOfBirth.length() >= 10)
            {
                string tanggal = storedDateOfBirth.substr(0, 2);
                string bulan = storedDateOfBirth.substr(3, 2);
                string tahun = storedDateOfBirth.substr(6, 4);
                cout << "TTL: " << tanggal << "-" << bulan << "-" << tahun << " (Tanggal-Bulan-Tahun)" << endl;
            }
            else
            {
                cout << "TTL: Data tidak lengkap" << endl;
            }
            break;
        }
    }
    infile.close();

    if (!found)
    {
        cout << "Nama pengguna tidak ditemukan.\n";
        return;
    }

    int pilihanAkun;
    cout << "Pengaturan Akun:\n";
    cout << "1. Ubah Data Diri\n";
    cout << "2. Ubah Kata Sandi\n";
    cout << "0. Kembali\n";
    cout << "Pilih opsi: ";
    cin >> pilihanAkun;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Bersihkan buffer

    switch (pilihanAkun)
    {
    case 1:
        system("cls");
        ubahDataDiri(username);
        break;
    case 2:
        system("cls");
        ubahPassword(username);
        break;
    case 0:
        break;
    default:
        cout << "Pilihan tidak valid.\n";
        break;
    }
    system("pause");
    system("cls");
}

int generatePesananID()
{
    static int id = 0;
    return ++id; // Pre-increment untuk memastikan ID dimulai dari 1
}

//MANAGE FILM 
// Linked List untuk Film
class FilmList
{
private:
    Film *head;
    string kodeFilm;
    string namaFilm;
    string genreFilm;
    string ratingFilm;
    float hargaFilm;

public:
    FilmList() : head(nullptr) {}

bool tambahFilm()
{
    char choice;
    do
    {
        system("cls");
        cout << "\n\t\t Masukkan Kode Film: ";
        cin >> kodeFilm;
        cin.ignore(); // Membersihkan buffer

        // Membuka file untuk membaca, dan membuatnya jika tidak ada
        fstream data;
        data.open("films.txt", ios::in);
        if (!data.is_open()) {
            // Membuat file jika tidak ada
            data.open("films.txt", ios::out);
            data.close();
            data.open("films.txt", ios::in);
        }

        string line;
        bool isDuplicate = false;

        while (getline(data, line))
        {
            if (line.find(kodeFilm + ",") != string::npos)
            {
                isDuplicate = true;
                break;
            }
        }
        data.close();

        if (isDuplicate)
        {
            cout << "\n\t\t Kode film sudah ada. Silakan coba lagi.\n";
            system("pause");
            continue; // Menggunakan continue untuk kembali ke awal loop
        }
        else
        {
            cout << "\n\t\t Masukkan Nama Film: ";
            getline(cin, namaFilm);
            cout << "\n\t\t Masukkan Genre Film: ";
            getline(cin, genreFilm);
            cout << "\n\t\t Masukkan Rating Film: ";
            getline(cin, ratingFilm);

            // Menambahkan film ke file
            data.open("films.txt", ios::app | ios::out);
            data << kodeFilm << "," << namaFilm << "," << genreFilm << "," << ratingFilm << "\n";
            data.close();

            cout << "\n\t\t Film berhasil ditambahkan!";
            cout << "\n\nApakah anda ingin menambahkan film lain? (Y/N): ";
            cin >> choice;
            choice = toupper(choice);
        }
    } while (choice == 'Y');

    return (choice == 'N');
}

    void hapusFilm()
    {
        system("cls");
        cout << "\n\t\t Masukkan Kode Film yang akan dihapus: ";
        cin >> kodeFilm;
        cin.ignore(); // Membersihkan buffer

        ifstream ini_file("films.txt");
        ofstream out_file("tempFilms.txt");
        string line;
        bool found = false;

        if (ini_file.is_open() && out_file.is_open())
        {
            while (getline(ini_file, line))
            {
                if (line.substr(0, line.find(',')) != kodeFilm)
                {
                    out_file << line << "\n";
                }
                else
                {
                    found = true;
                }
            }
            ini_file.close();
            out_file.close();

            if (found)
            {
                remove("films.txt");
                rename("tempFilms.txt", "films.txt");
                cout << "\n\t\t Film berhasil dihapus!\n";
            }
            else
            {
                cout << "\n\t\t Kode film tidak ditemukan.\n";
                remove("tempFilms.txt");
            }
        }
        else
        {
            cout << "Gagal membuka file.\n";
        }
    }

    void lihatDaftarFilm()
    {
        ifstream file("films.txt");
        string line;

        system("cls");
        if (file.is_open())
        {
            cout << left << setw(15) << "Kode" << setw(30) << "Nama Film" << setw(20) << "Genre" << "Rating\n";
            cout << "--------------------------------------------------------------------------------\n";
            while (getline(file, line))
            {
                stringstream ss(line);
                string kodeFilm, namaFilm, genreFilm, ratingFilm;
                getline(ss, kodeFilm, ',');
                getline(ss, namaFilm, ',');
                getline(ss, genreFilm, ',');
                getline(ss, ratingFilm, ',');

                cout << left << setw(15) << kodeFilm << setw(30) << namaFilm << setw(20) << genreFilm << ratingFilm << endl;
            }
            file.close();
        }
        else
        {
            cout << "Gagal membuka file.\n";
        }
    }

    void ubahDataFilm()
    {
        system("cls");
        cout << "\n\t\t Masukkan Kode Film yang akan diubah: ";
        cin >> kodeFilm;
        cin.ignore(); // Membersihkan buffer

        ifstream infile("films.txt");
        ofstream tempFile("tempFilms.txt");
        string line;
        bool found = false;

        if (infile.is_open() && tempFile.is_open())
        {
            while (getline(infile, line))
            {
                stringstream ss(line);
                string currentKode, currentNama, currentGenre, currentRating;
                getline(ss, currentKode, ',');
                getline(ss, currentNama, ',');
                getline(ss, currentGenre, ',');
                getline(ss, currentRating, ',');

                if (currentKode == kodeFilm)
                {
                    cout << "\n\t\t Masukkan Nama Film baru: ";
                    getline(cin, namaFilm);
                    cout << "\n\t\t Masukkan Genre Film baru: ";
                    getline(cin, genreFilm);
                    cout << "\n\t\t Masukkan Rating Film baru: ";
                    getline(cin, ratingFilm);

                    tempFile << kodeFilm << "," << namaFilm << "," << genreFilm << "," << ratingFilm << "\n";
                    found = true;
                }
                else
                {
                    tempFile << line << "\n";
                }
            }
            infile.close();
            tempFile.close();

            if (found)
            {
                remove("films.txt");
                rename("tempFilms.txt", "films.txt");
                cout << "\n\t\t Data film berhasil diubah.\n";
            }
            else
            {
                cout << "\n\t\t Kode film tidak ditemukan.\n";
                remove("tempFilms.txt");
            }
        }
        else
        {
            cout << "Gagal membuka file.\n";
        }
    }

    Film *getFilmByKode(const string &kode) {
        ifstream file("films.txt");
        string line;
        if (file.is_open()) {
            while (getline(file, line)) {
                stringstream ss(line);
                string currentKode, nama, genre, rating;
                getline(ss, currentKode, ',');
                if (currentKode == kode) {
                    Film *film = new Film;
                    getline(ss, film->nama, ',');
                    getline(ss, film->genre, ',');
                    getline(ss, film->rating);
                    film->kode = currentKode;
                    return film;
                }
            }
            file.close();
        }
        return nullptr; // Kembalikan nullptr jika tidak ada film yang cocok
    }

};
//MANAGE BIOKOP
// Linked List untuk Bioskop
class BioskopList
{
private:
    Bioskop *head;
    string namaBioskop;
    string alamatBioskop;
    float hargaTiket;

public:
    BioskopList() : head(nullptr) {}
int jumlahBioskop = 0;

bool tambahBioskop(const string& nama, const string& alamat, float hargaTiket, int jumlahStudio){
    char choice;
    do
    {
        system("cls");
        cout << "\n\t\t Masukkan Nama Bioskop: ";
        getline(cin, namaBioskop);
        cin.ignore(); // Membersihkan buffer
        fstream data;
        data.open("bioskops.txt", ios::in);
        string line;
        bool isDuplicate = false;

        while (getline(data, line))
        {
            if (line.find(namaBioskop + ",") != string::npos)
            {
                isDuplicate = true;
                break;
            }
        }
        data.close();

        if (isDuplicate)
        {
            cout << "\n\t\t Nama Bioskop sudah ada. Silakan coba lagi.\n";
            system("pause");
            return false; // Kembali ke menu admin
        }
        else
        {
            cout << "\n\t\t Masukkan Alamat Bioskop: ";
            getline(cin, alamatBioskop);
            cout << "\n\t\t Masukkan Jumlah Studio: ";
            cin >> jumlahStudio;
            cout << "\n\t\t Masukkan Harga Tiket: ";
            cin >> hargaTiket;

            // Menambahkan bioskop ke file
            data.open("bioskops.txt", ios::app | ios::out);
            data << namaBioskop << "," << alamatBioskop << "," << jumlahStudio << "," << hargaTiket << "\n";
            data.close();

            jumlahBioskop++;
            cout << "\n\t\t Bioskop berhasil ditambahkan!";
            cout << "\n\nApakah anda ingin menambahkan Bioskop lain? (Y/N): ";
            cin >> choice;
            choice = toupper(choice);
        }
    } while (choice == 'Y');

    return (choice == 'N');
}
    void hapusBioskop(string nama)
    {
        system("cls");
        cout << "\n\t\t Masukkan Kode Bioskops yang akan dihapus: ";
        cin >> namaBioskop;
        cin.ignore(); // Membersihkan buffer

        ifstream ini_file("bioskops.txt");
        ofstream out_file("tempBioskops.txt");
        string line;
        bool found = false;

        if (ini_file.is_open() && out_file.is_open())
        {
            while (getline(ini_file, line))
            {
                if (line.substr(0, line.find(',')) != namaBioskop)
                {
                    out_file << line << "\n";
                }
                else
                {
                    found = true;
                }
            }
            ini_file.close();
            out_file.close();

            if (found)
            {
                remove("bioskops.txt");
                rename("tempBioskops.txt", "bioskops.txt");
                cout << "\n\t\t Bioskop berhasil dihapus!\n";
            }
            else
            {
                cout << "\n\t\t Kode film tidak ditemukan.\n";
                remove("tempBioskops.txt");
            }
        }
        else
        {
            cout << "Gagal membuka file.\n";
        }
    }

    void ubahDataBioskop(string nama, string alamatBaru, int jumlahStudioBaru, float hargaTiketBaru)
    {
        ifstream infile("bioskops.txt");
        ofstream tempFile("tempBioskops.txt");
        string line;
        bool found = false;

        if (infile.is_open() && tempFile.is_open())
        {
            while (getline(infile, line))
            {
                stringstream ss(line);
                string namaBioskop, alamat;
                int jumlahStudio;
                float hargaTiket;
                getline(ss, namaBioskop, ',');
                getline(ss, alamat, ',');
                ss >> hargaTiket;

                if (namaBioskop == nama)
                {
                    tempFile << namaBioskop << "," << alamatBaru << "," << "," << hargaTiketBaru << "\n";
                    found = true;
                }
                else
                {
                    tempFile << namaBioskop << "," << alamat << "," << "," << hargaTiket << "\n";
                }
            }
            infile.close();
            tempFile.close();
            remove("bioskops.txt");
            rename("tempBioskops.txt", "bioskops.txt");
            if (found)
            {
                cout << "Data bioskop " << nama << " berhasil diubah.\n";
            }
            else
            {
                cout << "Bioskop tidak ditemukan.\n";
            }
        }
        else
        {
            cout << "Gagal membuka file.\n";
        }
    }

    void lihatDaftarBioskop()
    {
        ifstream file("bioskops.txt");
        string line;
        int index = 1;

        if (file.is_open())
        {
            cout << left << setw(5) << "No" << setw(30) << "Nama Bioskop" << setw(30) << "Alamat" << setw(15) << "Jumlah Studio" << setw(10) << "Harga Tiket\n";
            cout << "----------------------------------------------------------------------------------------------------\n";
            while (getline(file, line))
            {
                stringstream ss(line);
                string nama, alamat;
                int jumlahStudio;
                float hargaTiket;
                getline(ss, nama, ',');
                getline(ss, alamat, ',');
                ss >> jumlahStudio;
                ss.ignore(1); // Mengabaikan koma
                ss >> hargaTiket;
                cout << left << setw(5) << index++ << setw(30) << nama << setw(30) << alamat << setw(15) << jumlahStudio << setw(10) << hargaTiket << endl;
            }
            file.close();
        }
        else
        {
            cout << "Gagal membuka file.\n";
        }
    }

    Bioskop *getBioskopByIndex(int index)
    {
        ifstream file("bioskops.txt");
        string line;
        int currentIndex = 0;
        Bioskop *bioskop = nullptr;

        if (file.is_open())
        {
            while (getline(file, line))
            {
                if (currentIndex == index)
                {
                    bioskop = new Bioskop;
                    stringstream ss(line);
                    getline(ss, bioskop->nama, ',');
                    getline(ss, bioskop->alamat, ',');
                    ss >> bioskop->jumlahStudio;
                    ss.ignore(1); // Mengabaikan koma
                    ss >> bioskop->hargaTiket;
                    break;
                }
                currentIndex++;
            }
            file.close();
        }
        return bioskop; // Jika indeks di luar jangkauan atau file tidak dapat dibuka, kembalikan nullptr
    }

    Bioskop* cariBioskop(BioskopList &list, const string &nama) {
        Bioskop *current = list.head;
        while (current != nullptr) {
            if (current->nama == nama) {
                return current;
            }
            current = current->next;
        }
        return nullptr;
    }
};

// MANAGE SEAT
class SeatManager {
public:
    void seat_display(int &y, int &x, char seat[4][10]) {
        cout << "   ";
        for (int screen = 0; screen <= 15; screen++) {
            cout << (char)254;
        }
        cout << " SCREEN ";
        for (int screen = 0; screen <= 15; screen++) {
            cout << (char)254;
        }
        cout << endl;

        // Menampilkan baris kursi dengan huruf A, B, C, D
        for (int y = 0; y < 4; y++) {
            cout << setw(2) << char('A' + y) << " "; // Menggunakan huruf untuk indeks baris
            for (int x = 0; x < 10; x++) {
                cout << (char)179 << " " << seat[y][x] << " ";
            }
            cout << endl;
        }

        // Menampilkan garis bawah
        cout << "   ";
        for (int w = 1; w <= 40; w++) {
            cout << (char)205;
        }
        cout << endl;
    }

    void seat_select(int &y, int &x, char seat[4][10]) {
        char row;
        int col;
        cout << "PILIH KURSI ANDA\n";
        cout << "BARIS (A-D): ";
        cin >> row;
        row = toupper(row); // Mengonversi input ke huruf besar

        // Memastikan input baris adalah huruf A, B, C, atau D
        while (row < 'A' || row > 'D') {
            cout << "Input tidak valid! Masukkan baris dari A hingga D.\n";
            cout << "BARIS (A-D): ";
            cin >> row;
            row = toupper(row);
        }

        y = row - 'A';

        cout << "KOLOM (0-9): ";
        cin >> col;

        // Loop untuk memilih kursi
        do {
            if (col < 0 || col > 9 || seat[y][col] == '1' || seat[y][col] == 'X') {
                cout << "Input tidak valid atau kursi sudah terisi! Coba lagi.\n";
                cout << "KOLOM (0-9): ";
                cin >> col;
            } else {
                seat[y][col] = '1'; // Menandai kursi sebagai terisi
                break;              // Keluar dari loop jika pemilihan kursi sukses
            }
        } while (true);

        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Bersihkan buffer input
    }

    void seat_select_agn(int &y, int &x, char seat[4][10]) {
        char seat_agn;
        do {
            system("cls");
            seat_display(y, x, seat);
            cout << "   \"1\" = Kursi Dipilih\n   \"X\" = Kursi sudah Terjual\n         ";
            cout << endl;
            cout << "Apakah Anda ingin memilih kursi lagi?\n";
            cout << "[Y] YA\n[N] TIDAK\n";
            cin >> seat_agn;
            seat_agn = toupper(seat_agn);

            if (seat_agn == 'Y') {
                seat_select(y, x, seat); // Memindahkan pemanggilan fungsi seat_select ke sini
                system("cls");
            } else if (seat_agn != 'Y' && seat_agn != 'N') {
                cout << "Input tidak valid! Silakan masukkan huruf yang benar!\n";
            }
        } while (seat_agn == 'Y'); // Loop hanya berlanjut jika pengguna memilih 'Y'
    }

    void check_class(int &y, int &x, char seat[4][10]) {
        system("cls");
        int total_seat_select = 0;
        for (y = 0; y < 4; y++) {
            for (x = 0; x < 10; x++) {
                if (seat[y][x] == '1') {
                    ++total_seat_select;
                }
            }
        }
        if (total_seat_select == 1) {
            cout << "\nAnda telah memilih " << total_seat_select << " kursi.\n\n";
        } else if (total_seat_select == 0) {
            cout << "\nAnda belum memilih kursi.\n\n";
        } else {
            cout << "\nAnda telah memilih " << total_seat_select << " kursi.\n\n";
        }
    }

    void konfirmasiKursi(char seat[4][10]) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 10; j++) {
                if (seat[i][j] == '1') {
                    seat[i][j] = 'X'; // Konfirmasi kursi sebagai terjual
                }
            }
        }
    }

    void simpanStatusKursi(const string& namaBioskop, const string& kodeFilm, int studio, const string& jamTayang, char seat[4][10]) {
        string filename = "status_kursi_" + namaBioskop + "_" + kodeFilm + "_studio" + to_string(studio) + "_jam" + jamTayang + ".txt";
        ofstream file(filename);
        if (file.is_open()) {
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 10; j++) {
                    file << seat[i][j];
                }
                file << "\n";
            }
            file.close();
        } else {
            cout << "Gagal menyimpan status kursi.\n";
        }
    }

    void muatStatusKursi(const string& namaBioskop, const string& kodeFilm, int studio, const string& jamTayang, char seat[4][10]) {
        string filename = "status_kursi_" + namaBioskop + "_" + kodeFilm + "_studio" + to_string(studio) + "_jam" + jamTayang + ".txt";
        ifstream file(filename);
        if (file.is_open()) {
            string line;
            int i = 0;
            while (getline(file, line) && i < 4) {
                for (int j = 0; j < 10; j++) {
                    seat[i][j] = line[j];
                }
                i++;
            }
            file.close();
        } else {
            cout << "File status kursi tidak ditemukan, mengasumsikan semua kursi kosong.\n";
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 10; j++) {
                    seat[i][j] = '0'; // Asumsi kursi kosong jika file tidak ditemukan
                }
            }
        }
    }

    void muatStatusKursiDariRiwayat(const string& bioskopNama, const string& jamTayang, int studio, char seat[4][10]) {
        ifstream file("riwayat_pesanan.txt");
        string line;
        if (file.is_open()) {
            while (getline(file, line)) {
                stringstream ss(line);
                string id, user, film, bioskop, studioStr, jam, bangku, harga, pembayaran;
                getline(ss, id, '|');
                getline(ss, user, '|');
                getline(ss, film, '|');
                getline(ss, bioskop, '|');
                getline(ss, studioStr, '|');
                getline(ss, jam, '|');
                getline(ss, bangku, '|');
                getline(ss, harga, '|');
                getline(ss, pembayaran);

                if (bioskop == bioskopNama && jam == jamTayang && stoi(studioStr) == studio) {
                    for (size_t i = 0; i < bangku.length(); i += 3) { // Misal format bangku "A1 B3 C2"
                        int row = bangku[i] - 'A';
                        int col = bangku[i+1] - '1';
                        seat[row][col] = 'X'; // Tandai sebagai terisi
                    }
                }
            }
            file.close();
        }
    }
};
// Queue untuk Pesanan
class PesananQueue
{
private:
    PesananNode *front;
    PesananNode *rear;

public:
    PesananQueue() : front(nullptr), rear(nullptr) {}

    void tambahPesanan(Pesanan *pesanan)
    {
        enqueue(pesanan);
    }

    void prosesPesanan()
    {
        if (front == nullptr)
        {
            cout << "Tidak ada pesanan dalam antrian.\n";
            return;
        }
        PesananNode *temp = front;
        front = front->next;
        if (front == nullptr)
        {
            rear = nullptr;
        }
        cout << "Memproses Pesanan ID: " << temp->pesanan->id << endl;
        delete temp;
    }

    void enqueue(Pesanan *pesanan)
    {
        PesananNode *newNode = new PesananNode{pesanan, nullptr}; // Memperbaiki pembuatan node baru
        if (rear == nullptr)
        {                           // Jika antrian kosong
            front = rear = newNode; // Node baru adalah depan dan belakang
        }
        else
        {
            rear->next = newNode; // Menambahkan node baru di belakang
            rear = newNode;       // Memperbarui node belakang
        }
    }  

    void simpanPesananKeFile(Pesanan* pesanan) {
    ofstream file("riwayat_pesanan.txt", ios::app); // Buka file dalam mode append
    if (file.is_open()) {
        file << pesanan->id << "|"
             << pesanan->user << "|"
             << pesanan->film->nama << "|"
             << pesanan->bioskop->nama << "|"
             << pesanan->studio << "|"
             << pesanan->jam << "|"
             << pesanan->bangku << "|"
             << pesanan->harga << "|"
             << pesanan->pembayaran << "\n";
        file.close();
    } else {
        cout << "Gagal membuka file riwayat pesanan.\n";
    }
}

    void tampilkanRiwayatPesanan() {
    ifstream file("riwayat_pesanan.txt");
    string line;

    if (file.is_open()) {
        cout << left << setw(10) << "ID Pesanan" << setw(20) << "Nama Pengguna" << setw(20) << "Film" << setw(15) << "Bioskop" << setw(10) << "Studio" << setw(15) << "Jam Tayang" << setw(10) << "Bangku" << setw(10) << "Harga" << setw(15) << "Pembayaran" << endl;
        cout << "----------------------------------------------------------------------------------------------------------------------------------------------------------------\n";
        while (getline(file, line)) {
            stringstream ss(line);
            string id, user, film, bioskop, studio, jam, bangku, harga, pembayaran;
            getline(ss, id, '|');
            getline(ss, user, '|');
            getline(ss, film, '|');
            getline(ss, bioskop, '|');
            getline(ss, studio, '|');
            getline(ss, jam, '|');
            getline(ss, bangku, '|');
            getline(ss, harga, '|');
            getline(ss, pembayaran);

            cout << left << setw(10) << id << setw(20) << user << setw(20) << film << setw(15) << bioskop << setw(10) << studio << setw(15) << jam << setw(10) << bangku << setw(10) << harga << setw(15) << pembayaran << endl;
        }
            file.close();
        } else {
        cout << "Gagal membuka file riwayat pesanan.\n";
        }
    }  
};
// Stack untuk Penyimpanan Struk Pesanan
class PesananStack
{
private:
    PesananStackNode *top;

public:
    PesananStack() : top(nullptr) {}
    void tambahPesanan(Pesanan *pesanan)
    {
        PesananStackNode *newNode = new PesananStackNode{pesanan, top};
        top = newNode;
    }

    void push(Pesanan *pesanan)
    {
        PesananStackNode *newNode = new PesananStackNode{pesanan, top}; // Memperbaiki pembuatan node baru
        top = newNode;                                                  // Memperbarui top menjadi node baru
    }

    void lihatStrukPesanan()
    {
        if (top == nullptr)
        {
            cout << "Tidak ada struk pesanan yang tersimpan.\n";
            return;
        }
        PesananStackNode *node = top;
        while (node != nullptr)
        {
            Pesanan *pesanan = node->pesanan;
            cout << left;
            cout << setw(50) << " ______________________________________________________" << endl;
            cout << setw(50) << "|                    STRUK PESANAN                     |" << endl;
            cout << setw(50) << "|                       NESANEMA                       |" << endl;
            cout << setw(50) << "|______________________________________________________|" << endl;
            cout << setw(30) << "|ID Pesanan         : " << setw(25) << pesanan->id << "|" << endl;
            cout << setw(50) << "|______________________________________________________|" << endl;
            cout << setw(30) << "|Nama User          : " << setw(25) << pesanan->user << "|" << endl;
            cout << setw(50) << "|______________________________________________________|" << endl;
            cout << setw(30) << "|Film               : " << setw(25) << pesanan->film->nama << "|" << endl;
            cout << setw(50) << "|______________________________________________________|" << endl;
            cout << setw(30) << "|Jam Tayang         : " << setw(25) << pesanan->jam << "|" << endl;
            cout << setw(50) << "|______________________________________________________|" << endl;
            cout << setw(30) << "|Bioskop            : " << setw(25) << pesanan->bioskop->nama << "|" << endl;
            cout << setw(50) << "|______________________________________________________|" << endl;
            cout << setw(30) << "|Studio             : " << setw(25) << pesanan->studio << "|" << endl;
            cout << setw(50) << "|______________________________________________________|" << endl;
            cout << setw(30) << "|Bangku             : " << setw(25) << pesanan->bangku << "|" << endl;
            cout << setw(50) << "|______________________________________________________|" << endl;
            cout << setw(30) << "|Harga              : " << fixed << setprecision(2) << setw(25) << pesanan->harga << "|" << endl;
            cout << setw(50) << "|______________________________________________________|" << endl;
            cout << setw(30) << "|Metode Pembayaran  : " << setw(25) << pesanan->pembayaran << "|" << endl;
            cout << setw(50) << "|______________________________________________________|" << endl
                 << endl;
            node = node->next;
        }
    }
};

void tampilkanDataUser() {
    ifstream infile("pengguna.txt");
    User users[100]; // Array untuk menyimpan data pengguna, asumsikan maksimal 100 pengguna
    string line;
    int count = 0;

    while (getline(infile, line) && count < 100) {
        stringstream ss(line);
        string userType, username, password, securityQuestion, securityAnswer, ttl, nama, kotaAsal;
        getline(ss, userType, '|');
        getline(ss, username, '|');
        getline(ss, password, '|');
        getline(ss, securityQuestion, '|');
        getline(ss, securityAnswer, '|');
        getline(ss, ttl, '|');
        getline(ss, nama, '|');
        getline(ss, kotaAsal, '|');

        users[count].username = username;
        users[count].nama = nama;
        users[count].ttl = ttl;
        users[count].kotaAsal = kotaAsal;
        count++;
    }
    infile.close();

    // Implementasi bubble sort untuk sorting data pengguna berdasarkan username
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (users[j].username > users[j + 1].username) {
                // Tukar posisi
                User temp = users[j];
                users[j] = users[j + 1];
                users[j + 1] = temp;
            }
        }
    }

    // Tampilkan data pengguna dalam format tabel
    cout << left << setw(20) << "Nama" << setw(20) << "Username" << setw(20) << "Tanggal Lahir" << setw(20) << "Kota Asal" << endl;
    cout << string(80, '-') << endl; // Garis pemisah
    for (int i = 0; i < count; i++) {
        cout << left << setw(20) << users[i].nama << setw(20) << users[i].username << setw(20) << users[i].ttl << setw(20) << users[i].kotaAsal << endl;
    }
}
// Fungsi untuk menghasilkan ID pesanan unik
bool CariNamaPengguna(const string &namaPenggunaDicari)
{
    ifstream infile("pengguna.txt");
    string line;
    string userType, username, password, securityQuestion, securityAnswer, dateOfBirth, namaLengkap, kotaAsal;

    if (!infile.is_open())
    {
        cout << "Gagal membuka file pengguna.txt\n";
        return false;
    }

    while (getline(infile, line))
    {
        stringstream ss(line);
        getline(ss, userType, '|');
        getline(ss, username, '|');
        getline(ss, password, '|');
        getline(ss, securityQuestion, '|');
        getline(ss, securityAnswer, '|');
        getline(ss, dateOfBirth, '|');
        getline(ss, namaLengkap, '|');
        getline(ss, kotaAsal, '|');

        if (username == namaPenggunaDicari)
        {
            cout << "Nama pengguna ditemukan: " << username << endl;
            cout << "Nama Lengkap: " << namaLengkap << endl;
            cout << "Kota Asal: " << kotaAsal << endl;
            cout << "Tanggal Lahir: " << dateOfBirth << endl;
            infile.close();
            return true;
        }
    }

    infile.close();
    cout << "Nama pengguna tidak ditemukan.\n";
    return false;
}

bool dataBioskopDimuat = false;

void bacaBioskop() {
    if (!dataBioskopDimuat) {
        ifstream file("bioskops.txt");
        string line;
        while (getline(file, line) && jumlahBioskop < MAX_BIOSKOP) {
            stringstream ss(line);
            getline(ss, namaBioskop[jumlahBioskop], ',');
            jumlahBioskop++;
        }
        file.close();
        dataBioskopDimuat = true; // Set flag bahwa data sudah dimuat
    }
}

void inisialisasiGraph() {
    for (int i = 0; i < jumlahBioskop; i++) {
        for (int j = 0; j < jumlahBioskop; j++) {
            if (i == j) graph[i][j] = 0;
            else graph[i][j] = INF;
        }
    }

    graph[0][1] = 5; graph[1][0] = 5;
    graph[1][2] = 3; graph[2][1] = 3;
    graph[0][2] = 7; graph[2][0] = 7;
    graph[1][3] = 4; graph[3][1] = 4;
    graph[2][3] = 2; graph[3][2] = 2;
    graph[0][3] = 10; graph[3][0] = 10;
    graph[3][4] = 6; graph[4][3] = 6;
    graph[4][5] = 1; graph[5][4] = 1;
    graph[5][6] = 3; graph[6][5] = 3;
    graph[6][7] = 4; graph[7][6] = 4;
    graph[7][8] = 5; graph[8][7] = 5;
    graph[8][9] = 2; graph[9][8] = 2;
    graph[9][0] = 11; graph[0][9] = 11;

}

int minDistance(int dist[], bool sptSet[], int jumlahBioskop) {
    int min = INF, min_index = -1;
    for (int v = 0; v < jumlahBioskop; v++) {
        if (!sptSet[v] && dist[v] < min) {
            min = dist[v];
            min_index = v;
        }
    }
    return min_index;
}

void dijkstra(int src, int jumlahBioskop, string namaBioskop[], int graph[][MAX_BIOSKOP]) {
    int dist[MAX_BIOSKOP]; // Array untuk menyimpan jarak terpendek dari src ke i
    bool sptSet[MAX_BIOSKOP]; // sptSet[i] akan true jika vertex i termasuk dalam shortest path tree

    // Inisialisasi semua jarak sebagai INFINITE dan stpSet[] sebagai false
    for (int i = 0; i < jumlahBioskop; i++) {
        dist[i] = INF;
        sptSet[i] = false;
    }

    // Jarak vertex dari dirinya sendiri selalu 0
    dist[src] = 0;

    // Temukan jalur terpendek untuk semua vertex
    for (int count = 0; count < jumlahBioskop - 1; count++) {
        // Pilih vertex jarak minimum dari set vertex yang belum diproses
        int u = minDistance(dist, sptSet, jumlahBioskop);

        // Tandai vertex yang dipilih sebagai diproses
        sptSet[u] = true;

        // Update dist value dari vertex yang berdekatan
        for (int v = 0; v < jumlahBioskop; v++) {
            // Update dist[v] hanya jika tidak ada di sptSet, ada edge dari u ke v,
            // dan total weight dari path dari src ke v melalui u lebih kecil dari nilai saat ini dari dist[v]
            if (!sptSet[v] && graph[u][v] && dist[u] != INF && dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
            }
        }
    }

    // Tampilkan jarak terpendek yang dihitung
    cout << "Jarak terpendek dari " << namaBioskop[src] << " ke bioskop lainnya:\n";
    for (int i = 0; i < jumlahBioskop; i++) {
        if (dist[i] == INF)
            cout << namaBioskop[i] << ": Tidak terhubung\n";
        else
            cout << namaBioskop[i] << ": " << dist[i] << " km\n";
    }
}

void book(FilmList &filmList, BioskopList &bioskopList, PesananQueue &pesananQueue, PesananStack &pesananStack, User &user);

class UserHashTable {
private:
    struct UserNode {
        User* user;
        UserNode* next;

        UserNode(User* user) : user(user), next(nullptr) {}
    };

    UserNode** table; // Array of pointers to UserNode
    int capacity; // Maximum capacity of hash table

    int hashFunction(const string& key) {
        int hash = 0;
        for (char c : key) {
            hash = hash * 31 + c;
        }
        return hash % capacity;
    }

public:
    UserHashTable(int size) : capacity(size) {
        table = new UserNode*[capacity]();
    }

    ~UserHashTable() {
        for (int i = 0; i < capacity; i++) {
            UserNode* node = table[i];
            while (node != nullptr) {
                UserNode* temp = node;
                node = node->next;
                delete temp->user; // Assuming that the User objects are dynamically allocated
                delete temp;
            }
        }
        delete[] table;
    }

    void insertUser(User* user) {
        int index = hashFunction(user->username);
        UserNode* newNode = new UserNode(user);
        newNode->next = table[index];
        table[index] = newNode;
    }

    User* getUser(const string& username) {
        int index = hashFunction(username);
        UserNode* node = table[index];
        while (node != nullptr) {
            if (node->user->username == username) {
                return node->user;
            }
            node = node->next;
        }
        return nullptr; // User not found
    }

    void deleteUser(const string& username) {
        int index = hashFunction(username);
        UserNode* node = table[index];
        UserNode* prev = nullptr;
        while (node != nullptr) {
            if (node->user->username == username) {
                if (prev == nullptr) {
                    table[index] = node->next;
                } else {
                    prev->next = node->next;
                }
                delete node->user; // Assuming that the User objects are dynamically allocated
                delete node;
                return;
            }
            prev = node;
            node = node->next;
        }
    }
};

// Kelas untuk mengelola Pohon Biner
class BinaryTree {
private:
    TreeNode* root;

    // Fungsi rekursif untuk menambahkan node baru
    TreeNode* insert(TreeNode* node, int value) {
        if (node == nullptr) {
            return new TreeNode(value);
        }
        if (value < node->data) {
            node->left = insert(node->left, value);
        } else if (value > node->data) {
            node->right = insert(node->right, value);
        }
        return node;
    }

    // Fungsi rekursif untuk traversal inorder
    void inorder(TreeNode* node) {
        if (node != nullptr) {
            inorder(node->left);
            cout << node->data << " ";
            inorder(node->right);
        }
    }
    // Fungsi rekursif untuk mencari nilai dalam pohon
    bool search(TreeNode* node, int value) {
        if (node == nullptr) {
            return false;
        }
        if (node->data == value) {
            return true;
        }
        return value < node->data ? search(node->left, value) : search(node->right, value);
    }

public:
    BinaryTree() : root(nullptr) {}

    // Fungsi publik untuk menambahkan nilai
    void insert(int value) {
        root = insert(root, value);
    }

    // Fungsi publik untuk melakukan traversal inorder
    void displayInorder() {
        inorder(root);
        cout << endl;
    }

    // Fungsi publik untuk mencari nilai dalam pohon
    bool contains(int value) {
        return search(root, value);
    }
};

// Fungsi utama untuk menampilkan menu dan berinteraksi dengan user
void tampilkanMenuUser(FilmList &filmList, BioskopList &bioskopList, PesananQueue &pesananQueue, PesananStack &pesananStack, User &user)
{
    string username, friendUsername;
    int pilihan;

    while (true)
    {
        cout << R"(
                     ********** MENU USER **********
                     _______________________________
                    |                               |
                    | 1) Pesan Tiket                |
                    |                               |
                    | 2) Lihat Daftar Film          |
                    |                               |
                    | 3) Lihat Daftar Bioskop       |
                    |                               |
                    | 4) Lihat Struk Pesanan Tiket  |
                    |                               |
                    | 5) Jarak Bioskop              |
                    |                               |
                    | 6) Lihat Teman                |
                    |                               |
                    | 7) Pengaturan Akun            |
                    |                               |
                    | 0) Keluar                     |
                    |                               |
                    |_______________________________|
    )";
        cout << "\n\n\t\t\t Pilih opsi: ";
        cin >> pilihan;
        system("cls");

        switch (pilihan)
        {
        case 1:
        {
            book(filmList, bioskopList, pesananQueue, pesananStack, user);
            break;
        }
        case 2:
        {
            filmList.lihatDaftarFilm();
            system("pause");
            system("cls");
            break;
        }
        case 3:
        {
            bioskopList.lihatDaftarBioskop();
            system("pause");
            system("cls");
            break;
        }
        case 4:
        {
            pesananStack.lihatStrukPesanan();
            system("pause");
            system("cls");
            break;
        }
        case 5:
        {
    bacaBioskop();
    inisialisasiGraph();

    int src;
    cout << "Pilih bioskop asal (0-" << jumlahBioskop-1 << "): ";
    cin >> src;

            dijkstra(src, jumlahBioskop, namaBioskop, graph);
            system("pause");
            fflush(stdout);
            system("cls");
            break;
        }
        case 6:
        {

            system("pause");
            system("cls");
            break;
        }
        case 7:
        {
            int pilihanAkun;
            cout << "Pengaturan Akun:\n";
            cout << "1. Ubah Kata Sandi\n";
            cout << "2. Ubah Data Diri\n";
            cout << "0. Kembali\n";
            cout << "Pilih opsi: ";
            cin >> pilihanAkun;
            if (pilihanAkun == 1)
            {
                ubahPassword(username);
            }
            else if (pilihanAkun == 2)
            {
                ubahDataDiri(username);
            }
            else if (pilihanAkun == 0)
            {
                break;
            }
            else
            {
                cout << "Pilihan tidak valid.\n";
            }
            system("pause");
            system("cls");
            break;
        }
        case 0:
        {
            system("cls");
            return;
        }
        default:
        {
            cout << "Pilihan tidak valid.\n";
            break;
        }
        }
    }
}

void book(FilmList &filmList, BioskopList &bioskopList, PesananQueue &pesananQueue, PesananStack &pesananStack, User &user) {
    string kodeFilm, bioskopIndex, jam, bangku, pembayaran;
    SeatManager seatManager;    
    char seat[4][10]; // Array untuk menyimpan status kursi
    int studio, y = 0, x = 0; // Deklarasi variabel y dan x untuk indeks kursi
    Film *film = nullptr;
    Bioskop *bioskop = nullptr;
    string seatChoice;
    int jumlahKursiDipilih = 0;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 10; j++) {
            seat[i][j] = '0';
        }
    }
    
    filmList.lihatDaftarFilm();
    cout << "Masukkan kode film: ";
    cin >> kodeFilm;
    film = filmList.getFilmByKode(kodeFilm);
    if (film == nullptr) {
        cout << "Film tidak ditemukan. Pastikan Anda memasukkan kode yang benar.\n";
        system("cls");
        return;
    }
    system("cls");
    bioskopList.lihatDaftarBioskop();
    cout << "Pilih Bioskop (masukkan nomor): ";
    cin >> bioskopIndex;
    int index = stoi(bioskopIndex) - 1;
    bioskop = bioskopList.getBioskopByIndex(index);
    if (bioskop == nullptr) {
        cout << "Bioskop tidak ditemukan.\n";
        return;
    }
    system("cls");
    seatManager.muatStatusKursi(bioskop->nama, film->kode, studio, jam, seat);
    seatManager.muatStatusKursiDariRiwayat(bioskop->nama, jam, studio, seat); // Memuat status kursi dari riwayat pesanan


     cout << "Pilih Jam Tayang: \n";
     // Menampilkan pilihan jam tayang
     const int jumlahJam = 5;
     string jamTayang[jumlahJam] = {"10:00", "13:00", "16:00", "19:00", "22:00"};
     int pilihanJam;
     for (int i = 0; i < jumlahJam; i++) {
         cout << i+1 << ". " << jamTayang[i] << endl;
     }
     cout << "Pilih nomor jam tayang: ";
     cin >> pilihanJam;
     // Validasi pilihan
     while (pilihanJam < 1 || pilihanJam > jumlahJam) {
         cout << "Pilihan tidak valid, silakan masukkan nomor yang benar: ";
         cin >> pilihanJam;
     }
     jam = jamTayang[pilihanJam - 1]; 
    system("cls");
     cout << "Pilih Studio: \n";
     // Menampilkan pilihan studio
     const int jumlahStudio = bioskop->jumlahStudio; // Misalkan setiap bioskop memiliki jumlah studio yang berbeda
     for (int i = 1; i <= jumlahStudio; i++) {
         cout << i << ". Studio " << i << endl;
     }
     cout << "Pilih nomor studio: ";
     cin >> studio;
     // Validasi pilihan
     while (studio < 1 || studio > jumlahStudio) {
         cout << "Pilihan tidak valid, silakan masukkan nomor yang benar: ";
         cin >> studio;
     }
    system("cls");
    seatManager.seat_display(y, x, seat);
    seatManager.seat_select(y, x, seat);
    seatManager.seat_select_agn(y, x, seat);
    seatManager.check_class(y, x, seat); // Memeriksa jumlah kursi yang dipilih


    stringstream ss;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 10; j++) {
            if (seat[i][j] == '1') {
                ss << (char)('A' + i) << (j + 1) << " ";
                jumlahKursiDipilih++;
            }
        }
    }
    seatChoice = ss.str();

    // Inisialisasi totalHarga
    float totalHarga = jumlahKursiDipilih * bioskop->hargaTiket;

    cin.ignore();
    cout << "Metode Pembayaran (Cash/Card): ";
    getline(cin, pembayaran);

    // Simpan pesanan
    Pesanan* pesanan = new Pesanan{generatePesananID(), user.nama, film, jam, bioskop, studio, seatChoice, totalHarga, pembayaran, ""};
    pesananQueue.simpanPesananKeFile(pesanan);
    pesananQueue.enqueue(pesanan);
    pesananStack.push(pesanan);
    
    seatManager.konfirmasiKursi(seat);
    seatManager.simpanStatusKursi(bioskop->nama, film->kode, studio, jam, seat);

    cout << "Pemesanan tiket berhasil.\n";

    cout << "Total harga: " << totalHarga << "\n"; // Menampilkan total harga

    system("pause");
}

void tampilkanMenuAdmin(FilmList &filmList, BioskopList &bioskopList, PesananQueue &pesananQueue, PesananStack &pesananStack)
{
    FilmList daftarFilm;
    BioskopList daftarBioskop;
    PesananQueue antrianPesanan;
    PesananStack strukPesanan;

    int pilihan;
    while (true)
    {
        cout << R"(
                     ********** MENU ADMIN **********
                     _______________________________
                    |                               |
                    | 1) Data Film                  |
                    |                               |
                    | 2) Data Bioskop               |
                    |                               |
                    | 3) Riwayat Pesanan            |
                    |                               |
                    | 4) Tampilkan User             |
                    |                               |
                    | 5) Cari User                  |
                    |                               |
                    | 0) Exit                       |
                    |                               |
                    |_______________________________|
    )";
        cout << "\n\n\t\t\t Pilih opsi: ";
        cin >> pilihan;
        system("cls");

        switch (pilihan)
        {
        case 1:
        {
            int subPilihan;
            cout << R"(
                        *** EDIT DATA FILM ***
                         _______________________
                        |                       |
                        | 1) Tambah Film        |
                        |                       |
                        | 2) Hapus Film         |
                        |                       |
                        | 3) Ubah Data Film     |
                        |                       |
                        | 4) Lihat Daftar Film  |
                        |                       |
                        | 0) Kembali            |
                        |                       |
                        |_______________________|
    )";
            cout << "\n\n\t\t\t Pilih opsi: ";
            cin >> subPilihan;
            system("cls");
            if (subPilihan == 1)
            {
                filmList.tambahFilm();
                cout << "Film berhasil ditambahkan.\n";
                system("pause");
                system("cls");
            }
            else if (subPilihan == 2)
            {
                string nama;
                cout << "Masukkan nama film yang akan dihapus: ";
                cin.ignore();
                getline(cin, nama);
                filmList.hapusFilm();
                cout << "Film berhasil dihapus.\n";
                system("pause");
                system("cls");
            }
            else if (subPilihan == 3)
            {
                string nama, genre, rating;
                cout << "Masukkan nama film yang akan diubah: ";
                cin.ignore();
                getline(cin, nama);
                cout << "Masukkan genre baru: ";
                getline(cin, genre);
                cout << "Masukkan rating baru: ";
                getline(cin, rating);
                filmList.ubahDataFilm();
                cout << "Film berhasil diubah.\n";
                system("pause");
                system("cls");
            }
            else if (subPilihan == 4)
            {
                filmList.lihatDaftarFilm();
                system("pause");
                system("cls");
            }
            else if (subPilihan == 0)
            {
                system("cls");
                break;
            }
            else
            {
                cout << "Pilihan tidak valid.\n";
            }
            break;
        }
        case 2:
        {
            int subPilihan;
            cout << R"(
                        *** EDIT DATA BIOSKOP ***
                         __________________________
                        |                          |
                        | 1) Tambah Bioskop        |
                        |                          |
                        | 2) Hapus Bioskop         |
                        |                          |
                        | 3) Ubah Data Bioskop     |
                        |                          |
                        | 4) Lihat Daftar Bioskop  |
                        |                          |
                        | 0) Kembali               |
                        |                          |
                        |__________________________|
    )";
            cout << "\n\n\t\t\t Pilih opsi: ";
            cin >> subPilihan;
            system("cls");
            if (subPilihan == 1)
            {
                string nama, alamat;
                int jumlahStudio;
                float hargaTiket;
                bioskopList.tambahBioskop(nama, alamat, jumlahStudio, hargaTiket);
                cout << "Bioskop berhasil ditambahkan.\n";
                system("pause");
                system("cls");
            }
            else if (subPilihan == 2)
            {
                string nama;
                cout << "Masukkan nama bioskop yang akan dihapus: ";
                cin.ignore();
                getline(cin, nama);
                bioskopList.hapusBioskop(nama);
                cout << "Bioskop berhasil dihapus.\n";
                system("pause");
                system("cls");
            }
            else if (subPilihan == 3)
            {
                string nama, alamat;
                int jumlahStudio;
                float hargaTiket;
                cout << "Masukkan nama bioskop yang akan diubah: ";
                cin.ignore();
                getline(cin, nama);
                cout << "Masukkan alamat baru: ";
                getline(cin, alamat);
                cout << "Masukkan jumlah studio baru: ";
                cin >> jumlahStudio;
                cout << "Masukkan harga tiket baru: ";
                cin >> hargaTiket;
                bioskopList.ubahDataBioskop(nama, alamat, jumlahStudio, hargaTiket);
                cout << "Bioskop berhasil diubah.\n";
                system("pause");
                system("cls");
            }
            else if (subPilihan == 4)
            {
                bioskopList.lihatDaftarBioskop();
                system("pause");
                system("cls");
            }
            else if (subPilihan == 0)
            {
                system("cls");
                break;
            }
            else
            {
                cout << "Pilihan tidak valid.\n";
            }
            break;
        }
        case 3:
        {
            pesananQueue.tampilkanRiwayatPesanan();
            system("pause");
            system("cls");
            break;
        }
        case 4:
        {
            tampilkanDataUser();
            system("pause");
            system("cls");
            break;
        }
        case 5:
        {
            string namaPengguna;
            cout << "Masukkan nama pengguna yang ingin dicari: ";
            cin >> ws; // Mengabaikan whitespace yang tersisa di buffer input
            getline(cin, namaPengguna); // Mengambil input dari pengguna
        
            // Memanggil fungsi CariNamaPengguna
            bool ditemukan = CariNamaPengguna(namaPengguna);
            if (ditemukan) {
                cout << "Pengguna ditemukan dan datanya telah ditampilkan di atas.\n";
            } else {
                cout << "Pengguna tidak ditemukan.\n";
            }

            system("pause");
            system("cls");
            tampilkanMenuAdmin(filmList, bioskopList, pesananQueue, pesananStack);
        }
        case 0:
        {
            return;
        }
        default:
        {
            cout << "Pilihan tidak valid.\n";
            break;
        }
        }
    }
}
// Fungsi utama untuk menampilkan menu dan berinteraksi dengan user atau admin
int main()
{
    system("cls");

    MenuUtama();

    return 0;
}

void MenuUtama()
{
    system("cls");
    int pilihan;
    while (true)
    {
        cout << R"(
                    ================================
                        WELCOME TO UNESA CINEMA
                    ================================

                     ********* LOGIN MENU *********
                     ______________________________
                    |                              |
                    | 1) Masuk                     |
                    |                              |
                    | 2) Daftar                    |
                    |                              |
                    | 3) Pulihkan Kata Sandi       |
                    |                              |
                    | 0) Keluar                    |
                    |                              |
                    |______________________________|
    )";
        cout << "\n\n\t\t\t Pilih opsi: ";
        while (true)
        {
            cin >> pilihan;
            if (cin.fail())
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Input tidak valid. Silakan masukkan angka 1-4.\n";
            }
            else
            {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            }
        }

        switch (pilihan)
        {
        case 1:
            Masuk(1);
            break;
        case 2:
            Daftar();
            break;
        case 3:
            PulihkanKataSandi();
            break;
        case 0:
            KeluarProgram();
            break;
        default:
            cout << "Pilihan tidak valid. Silakan coba lagi.\n";
            MenuUtama();
            break;
        }
    }
    FilmList filmList;
    BioskopList bioskopList;
    PesananQueue pesananQueue;
    PesananStack pesananStack;
    User user;
    switch (pilihan)
    {
    case 1:
        Masuk(1);
        break;
    case 2:
        Daftar();
        break;
    case 3:
        PulihkanKataSandi();
        break;
    case 4:
        KeluarProgram();
        break;
    case 99:
        tampilkanMenuAdmin(filmList, bioskopList, pesananQueue, pesananStack); // Panggil dengan objek yang benar        default:
        cout << "Pilihan tidak valid. Silakan coba lagi.\n";
        MenuUtama();
        break;
    }
}

void Masuk(int attempt = 1)
{
    system("cls");
    cout << setw(50) << " ______________________________________________________" << endl;
    cout << setw(50) << "|                    LOGIN AKUN                        |" << endl;
    cout << setw(50) << "|                     NESANEMA                         |" << endl;
    cout << setw(50) << "|______________________________________________________|" << endl;
    cout << "Nama pengguna: ";
    cin >> username;
    BersihkanCin();

    cout << "Kata sandi: ";
    cin >> password;
    BersihkanCin();
    FilmList filmList;
    BioskopList bioskopList;
    PesananQueue pesananQueue;
    PesananStack pesananStack;
    User user;
    if (VerifikasiMasuk(username, password))
    {
        ifstream infile("pengguna.txt");
        string line, userType, storedUsername, storedPassword;
        bool isAdmin = false;
        while (getline(infile, line))
        {
            stringstream ss(line);
            getline(ss, userType, '|'); // Ambil userType sebelum username
            getline(ss, storedUsername, '|'); // Ambil username
            getline(ss, storedPassword, '|'); // Ambil password
            if (storedUsername == username && userType == "admin")
            {
                isAdmin = true;
                break;
            }
        }

        cout << "\nLOGIN BERHASIL!\nSelamat datang, " << username << "!\nTekan Enter untuk melanjutkan...";
        cin.get();
        if (isAdmin)
        {
            system("cls");
            tampilkanMenuAdmin(filmList, bioskopList, pesananQueue, pesananStack);
        }
        else
        {
            system("cls");
            user.nama = username; // Update nama pengguna di objek User
            tampilkanMenuUser(filmList, bioskopList, pesananQueue, pesananStack, user);
        }
    }
    else
    {
        cout << "\nNAMA PENGGUNA ATAU KATA SANDI SALAH!\n";
        if (attempt < 3)
        {
            cout << "Tekan Enter untuk coba lagi...";
            cin.get();
            Masuk(attempt + 1);
        }
        else
        {
            cout << "Anda telah mencoba 3 kali. Tekan Enter untuk kembali ke menu utama...";
            cin.get();
            MenuUtama();
        }
    }
}