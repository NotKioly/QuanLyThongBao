#include <iostream>
#include <vector>
#include <cctype>
#include <limits>
using namespace std;

// ================= VALIDATOR =================
class Validator {
public:
    static bool laEmail(string s) {
        int at = s.find('@');
        int dot = s.find('.', at);
        return (at > 0 && dot > at);
    }

    static bool laSDT(string s) {
        if (s.length() != 10 || s[0] != '0') return false;

        // kiểm tra ký tự số
        for (char c : s)
            if (!isdigit(c)) return false;

        // kiểm tra đầu số VN (3,5,7,8,9)
        if (s[1] != '3' && s[1] != '5' && s[1] != '7' && s[1] != '8' && s[1] != '9')
            return false;

        // không cho toàn số giống nhau
        bool allSame = true;
        for (int i = 1; i < 10; i++)
            if (s[i] != s[0]) allSame = false;
        if (allSame) return false;

        return true;
    }

    static bool laThoiGian(string s) {
        return (s.length() == 10 && s[2] == '/' && s[5] == '/');
    }
};

// ================= LỚP CƠ SỞ =================
class ThongBao {
protected:
    string tieuDe, noiDung, thoiGian;

public:
    virtual void nhap() {
        cout << "Tieu de: ";
        getline(cin, tieuDe);

        cout << "Noi dung: ";
        getline(cin, noiDung);

        do {
            cout << "Thoi gian (dd/mm/yyyy): ";
            getline(cin, thoiGian);
        } while (!Validator::laThoiGian(thoiGian));
    }

    virtual void xuat() = 0;
    virtual int tinhChiPhi() = 0;

    virtual ~ThongBao() {}
};

// ================= EMAIL =================
class ThongBaoEmail : public ThongBao {
private:
    string email;

public:
    void nhap() {
        ThongBao::nhap();
        do {
            cout << "Email: ";
            getline(cin, email);
        } while (!Validator::laEmail(email));
    }

    void xuat() {
        cout << "[EMAIL] " << email
             << " | " << tieuDe
             << " | Phi: " << tinhChiPhi() << " VND\n";
    }

    int tinhChiPhi() {
        return noiDung.length() * 10;
    }
};

// ================= SMS =================
class ThongBaoSMS : public ThongBao {
private:
    string sdt;

public:
    void nhap() {
        ThongBao::nhap();
        do {
            cout << "SDT: ";
            getline(cin, sdt);
        } while (!Validator::laSDT(sdt));
    }

    void xuat() {
        cout << "[SMS] " << sdt
             << " | " << tieuDe
             << " | Phi: " << tinhChiPhi() << " VND\n";
    }

    int tinhChiPhi() {
        return 500;
    }
};

// ================= PUSH =================
class ThongBaoPush : public ThongBao {
private:
    string deviceID;

public:
    void nhap() {
        ThongBao::nhap();
        cout << "Device ID: ";
        getline(cin, deviceID);
    }

    void xuat() {
        cout << "[PUSH] " << deviceID
             << " | " << tieuDe
             << " | Phi: " << tinhChiPhi() << " VND\n";
    }

    int tinhChiPhi() {
        return 100;
    }
};

// ================= MAIN =================
int main() {
    vector<ThongBao*> ds;
    int chon;

    do {
        cout << "\n======= MENU QUAN LY THONG BAO =======\n";
        cout << "1. Thong bao Email\n";
        cout << "2. Thong bao SMS\n";
        cout << "3. Thong bao Push\n";
        cout << "4. Thong ke chi phi\n";
        cout << "0. Thoat\n";
        cout << "Chon: ";

        cin >> chon;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // FIX getline

        ThongBao* tb = nullptr;

        if (chon == 1) tb = new ThongBaoEmail();
        else if (chon == 2) tb = new ThongBaoSMS();
        else if (chon == 3) tb = new ThongBaoPush();

        if (tb != nullptr) {
            tb->nhap();
            ds.push_back(tb);
        }

        if (chon == 4) {
            int tong = 0;
            cout << "\n===== DANH SACH =====\n";
            for (auto x : ds) {
                x->xuat();
                tong += x->tinhChiPhi();
            }
            cout << "TONG CHI PHI: " << tong << " VND\n";
        }

    } while (chon != 0);

    for (auto x : ds) delete x;

    return 0;
}