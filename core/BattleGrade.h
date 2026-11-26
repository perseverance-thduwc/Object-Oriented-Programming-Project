// File: BattleGrade.h
// Cham diem cuoi tran (Tuan 6): GradedActivity + CurvedActivity.
// CurvedActivity REDEFINING setScore (cung ten, cung tham so, KHONG virtual
// -> static binding, dung nhu bai giang Tuan 6).
#ifndef BATTLEGRADEHEADER
#define BATTLEGRADEHEADER

class GradedActivity {
protected:
    char letter;
    double score;
    void determineGrade() {
        if (score >= 90.0) letter = 'A';
        else if (score >= 80.0) letter = 'B';
        else if (score >= 70.0) letter = 'C';
        else if (score >= 60.0) letter = 'D';
        else letter = 'F';
    }
public:
    GradedActivity() : letter(' '), score(0.0) {}
    void setScore(double s) { score = s; determineGrade(); }
    double getScore() const { return score; }
    char getLetterGrade() const { return letter; }
};

class CurvedActivity : public GradedActivity {
protected:
    double rawScore;
    double percentage;   // he so duong cong (1.10 = +10%)
public:
    CurvedActivity() : GradedActivity(), rawScore(0.0), percentage(1.0) {}
    void setPercentage(double c) { percentage = c; }
    // REDEFINING: ap duong cong roi goi lai ham cua lop co so
    void setScore(double s) {
        rawScore = s;
        double curved = rawScore * percentage;
        if (curved > 100.0) curved = 100.0;
        GradedActivity::setScore(curved);
    }
    double getRawScore() const { return rawScore; }
};

#endif
