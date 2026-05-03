#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <limits>

using namespace std;

// ================= VALIDATOR (BỘ KIỂM TRA) =================
class Validator {
public:
    static bool laEmail(string s) {
        size_t at = s.find('@');
        size_t dot = s.find('.', at);
        return (at != string::npos && at > 0 && dot != string::npos && dot > at + 1);
    }

    static bool laSDT(string s) {
        // Kiểm tra độ dài và số 0 ở đầu
        if (s.length() != 10 || s[0] != '0') return false;

        // Kiểm tra ký tự số
        for (char c : s)
            if (!isdigit(c)) return false;

        // Kiểm tra đầu số VN (3, 5, 7, 8, 9)
        if (string("35789").find(s[1]) == string::npos)
            return false;

        // Kiểm tra nếu toàn bộ số giống hệt nhau (ví dụ: 0333333333)
        bool allSame = true;
        for (int i = 1; i < 10; i++) {
            if (s[i] != s[0]) {
                allSame = false;
                break;
            }
        }
        if (allSame) return false;

        return true;
    }

    static bool laThoiGian(string s) {
        // Định dạng dd/mm/yyyy
        if (s.length() != 10 || s[2] != '/' || s[5] != '/') return false;

        for (int i = 0; i < 10; i++) {
            if (i == 2 || i == 5) continue;
            if (!isdigit(s[i])) return false;
        }

        int d = stoi(s.substr(0, 2));
        int m = stoi(s.substr(3, 2));
        int y = stoi(s.substr(6, 4));

        if (y < 1 || m < 1 || m > 12) return false;

        int ngayTrongThang[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

        // Kiểm tra năm nhuận
        if ((y % 400 == 0) || (y % 4 == 0 && y % 100 != 0)) {
            ngayTrongThang[2] = 29;
        }

        return (d >= 1 && d <= ngayTrongThang[m]);
    }
};

// ================= LỚP CƠ SỞ (THÔNG BÁO) =================
class ThongBao {
protected:
    string tieuDe, noiDung, thoiGian;

public:
    virtual void nhap() {
        cout << "Tieu de: ";
        getline(cin, tieuDe);

        cout << "Noi dung: ";
        getline(cin, noiDung);

        while (true) {
            cout << "Thoi gian (dd/mm/yyyy): ";
            getline(cin, thoiGian);
            if (Validator::laThoiGian(thoiGian)) break;
            cout << "=> Ngay thang khong hop le. Vui long nhap lai!\n";
        }
    }

    virtual void xuat() = 0;
    virtual int tinhChiPhi() = 0;

    virtual ~ThongBao() {}
};

// ================= LỚP EMAIL =================
class ThongBaoEmail : public ThongBao {
private:
    string email;

public:
    void nhap() override {
        ThongBao::nhap();
        while (true) {
            cout << "Email: ";
            getline(cin, email);
            if (Validator::laEmail(email)) break;
            cout << "=> Email sai dinh dang (thieu @ hoac .). Vui long nhap lai!\n";
        }
    }

    void xuat() override {
        cout << "[EMAIL] " << email
             << " | Tieu de: " << tieuDe
             << " | Ngay: " << thoiGian
             << " | Phi: " << tinhChiPhi() << " VND\n";
    }

    int tinhChiPhi() override {
        return (int)noiDung.length() * 10;
    }
};

// ================= LỚP SMS =================
class ThongBaoSMS : public ThongBao {
private:
    string sdt;

public:
    void nhap() override {
        ThongBao::nhap();
        while (true) {
            cout << "SDT: ";
            getline(cin, sdt);
            if (Validator::laSDT(sdt)) break;
            cout << "=> SDT khong hop le (10 so, dung dau so VN, khong duoc trung lap hoan toan). Vui long nhap lai!\n";
        }
    }

    void xuat() override {
        cout << "[SMS]   " << sdt
             << " | Tieu de: " << tieuDe
             << " | Ngay: " << thoiGian
             << " | Phi: " << tinhChiPhi() << " VND\n";
    }

    int tinhChiPhi() override {
        return 500;
    }
};

// ================= LỚP PUSH =================
class ThongBaoPush : public ThongBao {
private:
    string deviceID;

public:
    void nhap() override {
        ThongBao::nhap();
        cout << "Device ID: ";
        getline(cin, deviceID);
    }

    void xuat() override {
        cout << "[PUSH]  " << deviceID
             << " | Tieu de: " << tieuDe
             << " | Ngay: " << thoiGian
             << " | Phi: " << tinhChiPhi() << " VND\n";
    }

    int tinhChiPhi() override {
        return 100;
    }
};

int main() {
    vector<ThongBao*> ds;
    int chon;

    do {
        cout << "\n======= MENU QUAN LY THONG BAO =======\n";
        cout << "1. Them Thong bao Email\n";
        cout << "2. Them Thong bao SMS\n";
        cout << "3. Them Thong bao Push\n";
        cout << "4. Xuat danh sach & Thong ke chi phi\n";
        cout << "0. Thoat\n";
        cout << "Chon: ";

        if (!(cin >> chon)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

        ThongBao* tb = nullptr;

        switch (chon) {
            case 1:
                tb = new ThongBaoEmail();
                break;
            case 2:
                tb = new ThongBaoSMS();
                break;
            case 3:
                tb = new ThongBaoPush();
                break;
            case 4: {
                int tong = 0;
                cout << "\n===== DANH SACH THONG BAO DA GUI =====\n";
                if (ds.empty()) {
                    cout << "(Trong)\n";
                } else {
                    for (auto x : ds) {
                        x->xuat();
                        tong += x->tinhChiPhi();
                    }
                    cout << "--------------------------------------\n";
                    cout << "TONG CHI PHI: " << tong << " VND\n";
                }
                break;
            }
            case 0:
                cout << "Dang thoat...\n";
                break;
            default:
                cout << "Lua chon khong hop le!\n";
        }

        if (tb != nullptr) {
            tb->nhap();
            ds.push_back(tb);
            cout << "=> Them thanh cong!\n";
        }

    } while (chon != 0);

    for (auto x : ds) delete x;
    ds.clear();

    return 0;
}
