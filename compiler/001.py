def countdown(n):
    print(n)
    if n == 0:
        return 0
    else:
        countdown(n - 1)

countdown(5)