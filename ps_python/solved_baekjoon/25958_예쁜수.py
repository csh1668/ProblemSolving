영, 일 = 0, 1
출력하랏, 숫자로바꿔, 각각에대해서, 입력받아랏, 다더해, 문자열로바꿔, 길이, 구간, 추가해랏 = print, int, map, input, sum, str, len, range, lambda x, y: x.append(y)

엠, 케이 = 각각에대해서(숫자로바꿔, 입력받아랏().split())
예쁜수들 = [영]
for 예쁜수후보 in 구간(일, 엠 + 일):
    각자리의합 = 다더해(각각에대해서(숫자로바꿔, 문자열로바꿔(예쁜수후보)))
    if 예쁜수후보 % 각자리의합 == 영:
        추가해랏(예쁜수들, 예쁜수후보)

경우의수입니당 = [[영] * (엠 + 일) for _ in 구간(길이(예쁜수들))]
for 예쁜수번호 in 구간(일, 길이(예쁜수들)):
    경우의수입니당[예쁜수번호][영] = 일
    예쁜수 = 예쁜수들[예쁜수번호]
    for 현재수 in 구간(일, 엠 + 일):
        경우의수입니당[예쁜수번호][현재수] += 경우의수입니당[예쁜수번호 - 일][현재수]
        if 현재수 >= 예쁜수:
            경우의수입니당[예쁜수번호][현재수] = (경우의수입니당[예쁜수번호][현재수] + 경우의수입니당[예쁜수번호][현재수 - 예쁜수]) % 케이

출력하랏(경우의수입니당[-일][-일])