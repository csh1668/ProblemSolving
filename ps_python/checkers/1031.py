import random

from dmoj.graders.interactive import InteractiveGrader, Interactor, WrongAnswer
from dmoj.result import CheckerResult, Result
from dmoj.problem import TestCase


number = 1

class Grader(InteractiveGrader):
    def interact(self, case: TestCase, interactor: Interactor):
        global number
        secret_number = number
        number += 1
        question_attempts = 0
        try:
            for i in range(11):
                op, n = interactor.readln().decode('utf-8').split()
                n = int(n)
                if op == '?':
                    if question_attempts >= 10:
                        raise WrongAnswer(f'질문을 10번 이상 했습니다.')
                    if n == secret_number:
                        interactor.writeln('YES')
                    else:
                        interactor.writeln('NO')
                    question_attempts += 1
                elif op == '!':
                    if n == secret_number:
                        return CheckerResult(True, 10, f'추측한 수 {n}가 정답과 일치합니다.')
                    else:
                        raise WrongAnswer(f'추측한 수 {n}이 정답 {secret_number}와 다릅니다.')
                else:
                    raise WrongAnswer(f'잘못된 입력입니다.')
        except Exception as e:
            raise WrongAnswer(f'채점 중 오류가 발생했습니다: {e}')