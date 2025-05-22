FROM gcc:latest

WORKDIR /app

# 필요한 개발 도구 설치
RUN apt-get update && \
    apt-get install -y cmake make gdb valgrind vim && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

# 소스 코드는 볼륨 마운트로 처리
CMD ["/bin/bash"]