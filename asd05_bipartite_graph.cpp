#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

int main()
{
    ios_base::sync_with_stdio(false);
    int participantsNo, mealNo, sampleNo;
    cin >> participantsNo >> mealNo >> sampleNo;
    int *participants = new int [participantsNo];
    pair<int, vector<int>> *meals = new pair<int, vector<int>>[mealNo];
    for (int i = 0; i < participantsNo; ++i)
    {
        participants[i] = -1;
    }
    for (int i = 0; i < mealNo; ++i)
    {
        meals[i].first = sampleNo;
    }

    vector<vector<int>> participantsAdjList(participantsNo);

    // Konstruowanie listy sąsiedztwa uczestników
    char c;
    for (int i = 0; i < participantsNo; ++i)
    {
        for (int j = 0; j < mealNo; ++j)
        {
            cin >> c;
            if (c == 'x')
            {
                participantsAdjList[i].push_back(j);
            }
        }
    }
    bool found = false;
    int currentMealParticipant, currentMeal, newMeal;
    // ALGORYTM
    // Spróbuj znaleźć posiłek dla każdego uczestnika festiwalu.
    for (int i = 0; i < participantsAdjList.size(); ++i)
    {
        found = false;
        // Przejrzyj wszystkie posiłki dla danej osoby i sprawdź czy jest taki, który można wybrać.
        for (int j = 0; j < participantsAdjList[i].size() && !found; ++j)
        {
            // Jeśli można wybrać, zmniejsz liczbę porcji posiłku i do listy w posiłku dodaj daną osobę.
            if (meals[ participantsAdjList[i][j] ].first > 0)
            {
                meals[ participantsAdjList[i][j] ].second.push_back(i);
                --meals[ participantsAdjList[i][j] ].first;
                participants[i] = participantsAdjList[i][j];
                found = true;
            }
        }
        // Jeśli nie znaleziono dla danej osoby porcji posiłku, szukaj ścieżek naprzemiennych.
        if (!found)
        {
            // Dla każdej krawędzi z uczestnika do posiłku (żadna nie jest skojarzona).
            for (int j = 0; j < participantsAdjList[i].size() && !found; ++j)
            {
                // Numer obecnie badanego posiłku.
                currentMeal = participantsAdjList[i][j];

                // Dla każdej skojarzonej krawędzi z posiłku do uczestnika.
                for (int k = 0; k < meals[currentMeal].second.size() && !found; ++k)
                {
                    // Numer uczestnika obecnie podpiętego pod posiłek currentMeal.
                    currentMealParticipant = meals[currentMeal].second[k];

                    // Dla każdej krawędzi z uczestnika do innych posiłków.
                    for (int h = 0; h < participantsAdjList[currentMealParticipant].size() && !found; ++h)
                    {
                        // Numer poszukiwanego nowego posiłku, z którym można skojarzyć uczestnika.
                        newMeal = participantsAdjList[currentMealParticipant][h];

                        // Jeśli istnieje wolna porcja posiłku i  krawędź nieskojarzona,
                        // skojarz posiłek z uczestnikiem currentMealParticipant.
                        if (meals[newMeal].first > 0 &&
                                find(meals[newMeal].second.begin(),
                                     meals[newMeal].second.end(),
                                     currentMealParticipant) == meals[newMeal].second.end())
                        {
                            meals[newMeal].second.push_back(currentMealParticipant);
                            --meals[newMeal].first;
                            participants[currentMealParticipant] = newMeal;
                            found = true;
                        }
                    }
                }
                // Skojarz uczestnika z posiłkiem.
                if (found)
                {
                    auto it = find(meals[currentMeal].second.begin(),
                                   meals[currentMeal].second.end(),
                                   currentMealParticipant);
                    meals[currentMeal].second.erase(it);
                    meals[currentMeal].second.push_back(i);
                    participants[i] = currentMeal;
                }
            }
        }
    }
    int result = 0;
    // Liczenie zadowolonych uczestników.
    for (int i = 0; i < participantsNo; ++i)
    {
        if (participants[i] != -1)
            result += 1;
    }
    cout << result << "\n";
    return 0;
}
