# FreedomRadioListViewer

.asx 파일에서 읽은 인터넷 라디오 채널에서 현재 방송 중인 음악의 Title과 Artist를 목록화해서 보여주는 프로그램입니다.

## 사용 기술
1. 라디오 채널 정보를 Shoutcast protocol로 받아오기 위한 TCP/IP 소켓 통신
2. 라디오 채널 목록을 STL Vector로 관리
3. 각 라디오 채널마다 Thread 사용
4. 라디오 채널 목록 2열 종대 형태로 표시하기 위한 Custom control 제작

## 특징
1. 현재 화면에 표시되고 있는 라디오 채널들만 5초마다 Refresh
2. 유효하지 않은 라디오 채널은 Refresh하지 않음

## 추후 추가 가능할지도 모르는 기능 목록
1. 라디오 직접 재생
2. 또는 팟플레이어를 이용한 간접 재생
