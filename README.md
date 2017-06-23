# ControlService
C++ Window Service ControlProgram

# Build
Need VisualStudio 2015

# Description

윈도우 서비스 프로그램으로 서비스가 아닌 일반 프로그램을 강제로 System권한 또는 일반 유저권한으로 실행시키는 기능과 프로세스가 죽었을때 강제 실행하는 기능도 있습니다.


간단 사용법 (예: 특정 프로그램 2개를 마치 서비스 프로그램처럼 윈도우 시작과 함께 System권한으로 항상 자동실행하기)

 1. SampleRoot폴더의 Control폴더를 C:\에 통채롤 복사
 2. RegService.exe를 관리자 권한으로 실행한뒤 Install버튼 실행
 3. ServiceInfo.ini를 편집 : CONTROLPROGRAM1 항목에 실행할 파일이 위치한 경로(PATH)와 실행파일 이름을(EXE) 넣어줍니다. PARAM란에는 실행파일을 그대로 넣거나 실행시 인자를 넣어야 하는경우 배치파일을 만들어 배치파일이름을 넣어줍니다
 4. WAKEOP항목에 1을 넣고 SYSTEM항목을 1로 설정하면 해당 프로그램이 마치 서비스 프로그램 같이 SYSTEM사용자로 실행됩니다 (0으로 설정시 일반 프로그램으로 실행됨)



# Project

1. RegService
- 서비스 프로그램을 등록 및 삭제 (관리자 권한으로 실행필요)

2. ServiceProgram
- 서비스 관련 클래스와 이벤트 처리 프로젝트

3. ServiceCommon
- Process관리나 스레드, 간단한 로그 기록 등 클래스들 모음