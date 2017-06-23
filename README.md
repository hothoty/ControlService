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
 5. CONTROLPROGRAM2 항목에 2번째 프로그램을 설정해줍니다 (3번째 4번째 프로그램도 동일하게 계속 추가해주기만 하면 추가됩니다)
 6. 작업 관리자를 열어 서비스 항목에서 ControlServiceProgram을 찾아서 시작시키면 4번에서 CONTROLPROGRAM에 등록된 프로그램들이 자동 실행됩니다



# Project

1. RegService
- 서비스 프로그램을 등록 및 삭제 (관리자 권한으로 실행필요)

2. ServiceProgram
- 서비스 관련 클래스와 이벤트 처리 프로젝트

3. ServiceCommon
- Process관리나 스레드, 간단한 로그 기록 등 클래스들 모음