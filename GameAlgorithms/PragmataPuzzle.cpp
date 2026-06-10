// PragmataPuzzle.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <chrono>

using namespace std;

constexpr uint8_t SlotEmpty = 0;
constexpr uint8_t SlotStart = 1;
constexpr uint8_t SlotDest = 2;
constexpr uint8_t SlotAStarOccupied = 3;
constexpr uint8_t SlotUnavailable = 4;
constexpr uint8_t SlotDamagePoint = 5;


struct FCoordPoint
{
public:
    FCoordPoint()
    {
        X = 0;
        Y = 0;
    };

    FCoordPoint(const int InX, const int InY)
    {
        X = InX;
        Y = InY;
    };

    FCoordPoint operator-(const FCoordPoint& Operand) const
    {
        FCoordPoint Cur = { X,Y };
        Cur.X -= Operand.X;
        Cur.Y -= Operand.Y;

        return Cur;
    }

    FCoordPoint operator+(const FCoordPoint& Operand) const
    {
        FCoordPoint Cur = { X,Y };
        Cur.X += Operand.X;
        Cur.Y += Operand.Y;
        
        return Cur;
    }

    bool operator==(const FCoordPoint& Other) const
    {
        return X == Other.X && Y == Other.Y;
    }

    int X;
    int Y;

};


//왼쪽,위,오른쪽,아래
const FCoordPoint Dir[4] = { FCoordPoint(-1,0), FCoordPoint(0,-1), FCoordPoint(1,0) , FCoordPoint(0,1) };

//Return Available(Empty) Positions.

bool CanReachEnd(vector<vector<uint8_t>>& Map, const FCoordPoint& InStartPoint, const FCoordPoint& InDestPoint,const uint8_t GridSize)
{
    FCoordPoint CountPoint = InStartPoint;

    queue<FCoordPoint> Queue;
    Queue.push(InStartPoint);

    vector<vector<bool>> bVisited(GridSize, vector<bool>(GridSize, false));

    while (Queue.size() != 0)
    {
        const FCoordPoint Cur = Queue.front();
        Queue.pop();
        
        //목적지 까지 잘 왔다면 별일 없는 것이다.
        if (Cur.X == InDestPoint.X && Cur.Y == InDestPoint.Y)
        {
            return true;
        }

        for (const FCoordPoint& Di : Dir)
        {
            const FCoordPoint Calc = Cur + Di;

            if (Calc.X <0 || Calc.X >= GridSize || Calc.Y <0 || Calc.Y >= GridSize)
            {
                continue;
            }

            if (bVisited[Calc.Y][Calc.X] == true)
            {
                continue;
            }

            if (Map[Calc.Y][Calc.X] == SlotUnavailable)
            {
                continue;
            }

            bVisited[Calc.Y][Calc.X] = true;
            Queue.push(Calc);
        }
    }

    return false;
}


/*
* UnavailableBlock을 설치 후 시작과 끝 경로까지 쿼리 후 도달 가능하면 넘어가고 아니면 다시함.
* 랜덤하게 지역을 Block 함. 
* 해당 지점에 
* BFS로 해당 지점까지 도달 가능한지 체크
* 
* 
*/

void GenerateSlotUnavailable(vector<vector<uint8_t>>& Map, const FCoordPoint& InStartPoint, const FCoordPoint& InDestPoint,const uint8_t GridSize)
{
    FCoordPoint Point;

    while (true)
    {
        Point = { rand() % GridSize, rand() % GridSize };

        if (Map[Point.Y][Point.X] != SlotEmpty)
        {
            continue;
        }

        Map[Point.Y][Point.X] = SlotUnavailable;

        if (CanReachEnd(Map, InStartPoint, InDestPoint,GridSize))
        {
            break;
        }
        else
        {
            Map[Point.Y][Point.X] = SlotEmpty;
        }
    }
}

void PragmataPuzzle()
{
    const int GridCount = 5;
    const uint8_t UnAvailableBlockCounts = 2; //TODO: 이 값에 대한 Validation 체크는 가능한 크기(예: 3*3이면 7칸만 가능함) 부터 체크한다
    const uint8_t WeaponUpgradeCounts = 3; //TODO: 이 값에 대한 Validation 체크는 위에서 계산한 가능한 크기에 UnAvailableBlockCounts를 포함하여 계산함.
    //그외 기타등등은 DP마냥 이전 결과를 누적하여 처리함

    vector<vector<uint8_t>> Map(GridCount, vector<uint8_t>(GridCount, 0));
    srand(chrono::system_clock::now().time_since_epoch().count());

    FCoordPoint DestPoint = FCoordPoint(rand() % GridCount, rand() % GridCount);
    Map[DestPoint.Y][DestPoint.X] = SlotDest;

    FCoordPoint StartPoint = FCoordPoint(rand() % GridCount, rand() % GridCount);
    while (Map[StartPoint.Y][StartPoint.X] != SlotEmpty)
    {
        StartPoint = FCoordPoint(rand() % GridCount, rand() % GridCount);
    }
    Map[StartPoint.Y][StartPoint.X] = SlotStart;

    GenerateSlotUnavailable(Map, StartPoint, DestPoint, GridCount);

    for (int y = 0; y < GridCount; ++y)
    {
        for (int x = 0; x < GridCount; ++x)
        {
            cout << (int)Map[y][x] << " ";
        }
        cout << endl;
    }
}

void WeightedRandom()
{
    const float weights[] = { 0.3,0.1,0.2,0.3,0.1};
    const int counts = sizeof(weights) / sizeof(float);
    srand(chrono::system_clock::now().time_since_epoch().count());

    int number = rand() % 100;
    float prop = number / 100.f;

    float counter = 0.f;

    for (int i = 0; i < counts; ++i)
    {
        counter += weights[i];
        if (prop < counter)
        {
            cout << "걸렸던 확률 : " << prop << endl;
            cout << "현재까지 누적: " << counter << endl;
            cout << "현재 인덱스" << i << endl;
            break;
        }
    }

    cout << endl;
    cout << endl;
}

int main()
{
    PragmataPuzzle();
    return 0;
}

// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.
