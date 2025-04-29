FROM gcc:latest

WORKDIR /app

# 필요한 개발 도구 설치
RUN apt-get update && \
    apt-get install -y cmake make gdb valgrind && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

# 소스 코드 복사
COPY . .

# 기본 명령어 설정 (컨테이너 실행 시 bash 쉘 시작)
CMD ["/bin/bash"]