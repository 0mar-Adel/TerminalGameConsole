// لعبة كسر الطوب بسيطة باستخدام SDL2
#include <SDL2/SDL.h>
#include <vector>
#include <iostream>
#include <cmath>
#include <cstdio> // لاستخدام snprintf لتحديث عنوان النافذة

// ثوابت الشاشة
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// خصائص اللعبة
const int PADDLE_WIDTH = 100;
const int PADDLE_HEIGHT = 20;
const int BALL_SIZE = 15;
const int BRICK_WIDTH = 80;
const int BRICK_HEIGHT = 30;
const int BRICK_ROWS = 5;
const int BRICK_COLS = 10;
const float BALL_SPEED_Y_INITIAL = 250.0f; // السرعة الرأسية الأولية للكرة
const float BALL_MAX_SPEED_X = 400.0f; // أقصى سرعة أفقية

// هياكل البيانات (Structs)
struct Ball {
    float x, y;
    float dx, dy;
};

struct Paddle {
    float x, y;
    float speed;
};

struct Brick {
    float x, y;
    bool active;
};

// دالة للتحقق من التصادم (Collision Detection)
bool checkCollision(SDL_Rect a, SDL_Rect b) {
    return SDL_HasIntersection(&a, &b);
}

// دالة لإعادة ضبط الكرة (عند الخسارة محاولة)
void resetBall(Ball& ball) {
    ball.x = (float)SCREEN_WIDTH / 2 - BALL_SIZE / 2;
    ball.y = (float)SCREEN_HEIGHT / 2;
    ball.dx = 0.0f;
    ball.dy = BALL_SPEED_Y_INITIAL;
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) return 1;

    SDL_Window* window = SDL_CreateWindow("Breakout Game | Lives: 3", 
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // متغيرات حالة اللعبة الجديدة
    bool running = true;
    bool gameWon = false;
    bool gameOver = false; // flag لحالة "انتهت اللعبة"
    int lives = 3;         // عدد المحاولات

    // إعداد الكائنات
    Paddle paddle = { (float)(SCREEN_WIDTH - PADDLE_WIDTH) / 2, (float)(SCREEN_HEIGHT - 50), 500.0f };
    Ball ball;
    resetBall(ball); // إعداد الكرة الأولية
    
    std::vector<Brick> bricks;
    int bricksCount = 0; // عداد الطوب الفعال

    for (int i = 0; i < BRICK_ROWS; ++i) {
        for (int j = 0; j < BRICK_COLS; ++j) {
            Brick b;
            b.x = j * BRICK_WIDTH;
            b.y = i * BRICK_HEIGHT + 50;
            b.active = true;
            bricks.push_back(b);
            bricksCount++;
        }
    }

    Uint32 lastTime = SDL_GetTicks();

    // حلقة اللعبة (Game Loop)
    while (running) {
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
            // يمكن إضافة زر لإعادة التشغيل هنا لو أردت
        }

        const Uint8* state = SDL_GetKeyboardState(NULL);

        // --- التحديث (Update Logic) ---
        
        // تحريك المضرب مسموح دائماً
        if (state[SDL_SCANCODE_LEFT] && paddle.x > 0) paddle.x -= paddle.speed * deltaTime;
        if (state[SDL_SCANCODE_RIGHT] && paddle.x < SCREEN_WIDTH - PADDLE_WIDTH) paddle.x += paddle.speed * deltaTime;
       
        // تحريك الكرة يتم فقط إذا لم يفز اللاعب أو يخسر
        if (!gameWon && !gameOver) {
            ball.x += ball.dx * deltaTime;
            ball.y += ball.dy * deltaTime;

            // 1. إطار الصد (اصطدام بالجدران الثلاثة)
            if (ball.x <= 0) ball.dx = fabs(ball.dx); // الجدار الأيسر
            if (ball.x + BALL_SIZE >= SCREEN_WIDTH) ball.dx = -fabs(ball.dx); // الجدار الأيمن
            if (ball.y <= 0) ball.dy = fabs(ball.dy); // الجدار العلوي

            // خسارة محاولة (الكرة وصلت للأسفل)
            if (ball.y >= SCREEN_HEIGHT) {
                lives--; // نقص المحاولة
                if (lives <= 0) {
                    gameOver = true;
                } else {
                    resetBall(ball); // إعادة ضبط الكرة لموقع البدء
                }
            }

            // تحديث حالة النافذة (العنوان)
            if (gameOver) {
                SDL_SetWindowTitle(window, "GAME OVER! (0 Lives) - Press ESC to exit");
            } else if (!gameWon) {
                char titleBuffer[100];
                snprintf(titleBuffer, sizeof(titleBuffer), "Breakout Game | Lives: %d | Bricks Left: %d", lives, bricksCount);
                SDL_SetWindowTitle(window, titleBuffer);
            }


            SDL_Rect ballRect = { (int)ball.x, (int)ball.y, BALL_SIZE, BALL_SIZE };
            SDL_Rect paddleRect = { (int)paddle.x, (int)paddle.y, PADDLE_WIDTH, PADDLE_HEIGHT };

            // فيزياء ارتداد المضرب (تغيير الزاوية حسب مكان الضربة)
            if (checkCollision(ballRect, paddleRect)) {
                float paddleCenter = paddle.x + PADDLE_WIDTH / 2.0f;
                float ballCenter = ball.x + BALL_SIZE / 2.0f;
                
                float offset = ballCenter - paddleCenter;
                
                // حساب السرعة الأفقية الجديدة. (قسمة Offset على نصف عرض المضرب، ثم ضربها بأقصى سرعة أفقية)
                ball.dx = (offset / (PADDLE_WIDTH / 2.0f)) * BALL_MAX_SPEED_X;
                
                // التأكد من أن الكرة ترتد للأعلى دائماً وتجنب التصاقها بالمضرب
                ball.dy = -fabs(ball.dy); 
            }

            // منطق تكسير الطوب والفوز
            for (auto& brick : bricks) {
                if (brick.active) {
                    SDL_Rect brickRect = { (int)brick.x, (int)brick.y, BRICK_WIDTH, BRICK_HEIGHT };
                    if (checkCollision(ballRect, brickRect)) {
                        brick.active = false;
                        ball.dy = -ball.dy; // عكس اتجاه الكرة
                        
                        bricksCount--;
                        if (bricksCount <= 0) {
                            gameWon = true;
                            SDL_SetWindowTitle(window, "YOU WON! GREAT JOB!");
                        }
                        break; 
                    }
                }
            }
        }

        // --- الرسم (Rendering) ---
        // تغيير الخلفية حسب حالة اللعبة
        if (gameOver) {
            SDL_SetRenderDrawColor(renderer, 100, 0, 0, 255); // أحمر داكن للخسارة
        } else if (gameWon) {
            SDL_SetRenderDrawColor(renderer, 0, 100, 0, 255); // أخضر داكن للفوز
        } else {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // أسود للوضع الطبيعي
        }
        SDL_RenderClear(renderer);

        // رسم المضرب والكرة فقط إذا لم تكن اللعبة منتهية بالخسارة
        if (!gameOver) {
            // رسم المضرب (أبيض)
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_Rect paddleDraw = { (int)paddle.x, (int)paddle.y, PADDLE_WIDTH, PADDLE_HEIGHT };
            SDL_RenderFillRect(renderer, &paddleDraw);

            // رسم الكرة (حمراء)
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_Rect ballDraw = { (int)ball.x, (int)ball.y, BALL_SIZE, BALL_SIZE };
            SDL_RenderFillRect(renderer, &ballDraw);
        }


        // رسم الطوب
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        for (const auto& brick : bricks) {
            if (brick.active) {
                SDL_Rect brickRect = { (int)brick.x + 2, (int)brick.y + 2, BRICK_WIDTH - 4, BRICK_HEIGHT - 4 };
                SDL_RenderFillRect(renderer, &brickRect);
            }
        }

        SDL_RenderPresent(renderer);
    }

    // تنظيف الذاكرة
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}