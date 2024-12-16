#ifndef LATEX_H_
#define LATEX_H_

enum latexError {
    NO_LATEX_ERRORS    = 0,
    LATEX_STREAM_ERROR = 1,
    WRITE_VALUE_ERROR  = 2,
    PHRASE_ERROR       = 3
};

#define writeBinaryOperation(operation)                                \
    if (rootNode->left) {                                              \
        internalWriteToLatex(diff, rootNode->left, latexStream);       \
    }                                                                  \
                                                                       \
    fprintf(latexStream, operation);                                   \
                                                                       \
    if (rootNode->right) {                                             \
        internalWriteToLatex(diff, rootNode->right, latexStream);      \
    }                                                           

#define writeUnaryOperation(operation, openBracket, closeBracket)      \
    fprintf(latexStream, operation);                                   \
    fprintf(latexStream, openBracket);                                 \
                                                                       \
    if (rootNode->left) {                                              \
        internalWriteToLatex(diff, rootNode->left, latexStream);       \
    }                                                                  \
                                                                       \
    fprintf(latexStream, closeBracket);                                

#define writeDerivating(rootNode, start, end)                          \
    fprintf(latexStream, start);                                       \
                                                                       \
    internalWriteToLatex(diff, rootNode, latexStream);                 \
                                                                       \
    fprintf(latexStream, end);                                         


// FUNCTION PROTOTYPES //
FILE *writeToLatex             (Differentiator *diff,     node<diffNode> *rootNode);
latexError internalWriteToLatex(Differentiator *diff,     node<diffNode> *rootNode, FILE *latexStream);
latexError writeValue          (node<diffNode> *rootNode, FILE                           *latexStream);
latexError writeHeader         (Differentiator *diff,     FILE                           *latexStream);
latexError writeEnding         (Differentiator *diff,     FILE                           *latexStream);
latexError writePhrase         (const char     **array,   FILE                           *latexStream, const size_t arraySize);
// FUNCTION PROTOTYPES //

static const char *head = "\\documentclass{article}\n"
                          "\\usepackage[english, russian]{babel}\n"
                          "\\usepackage[a4paper,top=2cm,bottom=2cm,left=1cm,right=1cm,marginparwidth=1.75cm]{geometry}\n"
                          "\\usepackage{amsmath}\n"
                          "\\usepackage{amsfonts}\n"
                          "\\usepackage{amssymb}\n"
                          "\\usepackage{graphicx}\n"
                          "\\usepackage[colorlinks=true, allcolors=blue]{hyperref}\n"
                          "\\usepackage{setspace}\n"
                          "\\usepackage{dashbox}\n"
                          "\\begin{document}\n"
                          "\\begin{Large}\n"
                          "\\begin{onehalfspace}\n"
                          "\\begin{center}\n"
                          "\\section*{\\huge ЭКЗАМЕНАЦИОННАЯ РАБОТА}\n"
                          "\\subsection*{\\large Дисциплина \\framebox{Введение в математический анализ}}\n"
                          "\\subsection*{Курс \\framebox{1} Семестр \\framebox{1} 2024-2025 учебный год}\n"
                          "\\subsection*{Фамилия и имя студента \\underline{Зубаха Максим}\\ \\hspace{2cm} № группы \\underline{Б05-431}}\n"
                          "\\begin{tabular}{|c|c|}\n"
                          "\\hline \n"
                          "Сумма баллов & \\hspace{8cm} \\\\ \n"
                          "\\hline \n"
                          "Оценка & \\hspace{8cm} \\\\ \n"
                          "\\hline \n"
                          "\\end{tabular} \n"
                          "\\vspace{1cm} \n"
                          "\\hline \n"
                          "\\end{center} \n"
                          "\\begin{large} \n"
                          "\\begin{enumerate} \n"
                          "\\item Найти $y^{(n)}$ при $n \\geq 3$. $$y = (x^2 - x + 1)\\ln(7-2x)$$ \n"
                          "\\hline \n"
                          "\\item Разложить функцию по формуле Тейлора при $x \\rightarrow -1$ до $o((x + 1)^{2n+1})$. $$y = (3x^2 + 6x + 5)e^{2x^2+4x-1}$$ \n"
                          "\\hline \n"
                          "\\item Найти предел. $$\\lim\\limits_{x \\rightarrow 0}(\\frac{3}{2} \\cdot \\frac{\\sin{x^2} - \\sin^2 x}{e^{x^2} - 1 - x^2})^{\\frac{1}{\\ch x - 1}}$$ \n"
                          "\\hline \n"
                          "\\item Вычислить производную, ответ упростить. \n";

static const char *endHead =  "\\end{enumerate}\n"
                              "\\end{large}\n";

static const char *end     =  "\\end{onehalfspace}\n"
                              "\\end{Large}\n"
                              "\\end{document}\n";

static const char *introduction     = "Уважаемый экзаменатор! Я бедный студент физтеха, который поступил сюда по ЕГЭ. "
                                      "Я сплю по 2 часа в сутки, в моей комнате уже накопилась огромная коллекция энергосов, "
                                      "которой бы хватило всем детям Африки на 20 лет. Все мои соседи говорят, что у меня проблемы с головой. "
                                      "Это странно, ведь соседей то у меня и нет. Один давно отчислился и ушел в армию, другой перевелся во ВШЭ, "
                                      "потому что решил, что он из этих, ну поняли. Сейчас у меня 3 пересдачи, я ботаю деда и матан 24/7. "
                                      "Товарищ экзаменатор, я не умею ничего, кроме как дифференцировать крокодилов. "
                                      "Сейчас я покажу вам все, чему научился за эти мучительные 4 месяца на физтехе... "
                                      "Разъебу этот 4-ый номер!";

static const char *derivatePhrase[] = {"Очевидно, что"
                                       "Заметим, что",
                                       "Мама сто раз говорила мне, что",
                                       "Сейчас вы удивитесь, но это правда, что",
                                       "Немного маханий руками и получим",
                                       "Мы все виноваты в этом пиздеце...",
                                       "Не стоит понимать этого",
                                       "Ниже представлен довольно тривиальный факт",
                                       "Вчера меня случайно занесло на гей-пати студентов ВШЭ, и они мне рассказали, что",
                                       "Такое уже встречалось в дебильнике по матану:",
                                       "Я это не понимаю, мне это неинтересно, но",
                                       "Студенты ПМИ вахуе от следующего преобразования",
                                       "Нам нужно строгое доказательство, поэтому",
                                       "Коллеги, прочувствуйте...",
                                       "Когда меня спрашивают, что для меня на втором месте после родителей, я говорю что",
                                       "Витамин С поможет понять тебе, что",
                                       "Ну это пиздец какой-то просто, ну сколько можно!",
                                       "Любой советский эмбрион знает, что",
                                       "Что это за пиздец такой? Ну как это может быть в 21м веке?",
                                       "Небольшой финт ушами...",
                                       "Да, я ошибся. Я могу один раз ошибиться. А нет, не ошибся...",
                                       "Это было не просто смело, это было пиздец как смело",
                                       "Здесь чувствуется какое-то чинопочитание, какое-то величие...",
                                       "Да, мне было страшно... Но я это сделал:",
                                       "Миш, мне похую, я так чувствую",
                                       "Здесь будем предельно аккуратными",
                                       "Сомниииительно, но окээй"};

static const size_t PHRASES_COUNT = sizeof(derivatePhrase) / sizeof(derivatePhrase[0]);

#endif // LATEX_H_
