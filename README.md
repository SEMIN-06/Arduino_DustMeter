<div align="center">
  <h3 align="center">휴대용 미세먼지 측정기</h3>

  <p align="center">
    아두이노와 Kodular(앱 인벤터) 앱을 사용하여 만든 휴대용 미세먼지 측정기
  </p>
</div>

<!-- ABOUT THE PROJECT -->

## 프로젝트 설명

2022년 11월 5일 진행 된 천안아산 청소년 인공지능 해커톤에서 만든 휴대용 미세먼지 측정기 입니다.
<br/>
아두이노와 스마트폰 앱을 블루투스로 연동하여 앱에서 미세먼지 수치를 확인할 수 있도록 하였고, 미세먼지 수치에 따라 도트 매트릭스에 표정이 나타나며, 이 표정을 앱에서 설정할 수 있도록 하였습니다.
<br/>
<br/>
아두이노 코드와 앱은 미리 개발하고, 해커톤 현장에서 스티로폼을 이용해 기계를 완성 시켰습니다.
</br>
당시 고등학교 1학년으로 안드로이드 앱에 대한 지식이 부족하여 안드로이드 앱은 Kodular (앱 인벤터) 라는 블럭 코딩 기반의 앱 제작 사이트를 이용했습니다.

### 팀원 설명

- **임세민: 팀장, 프로그래밍**
- 주민식: PPT 제작, 조립
- 김민재: PPT 제작
- 이진곤: PPT 제작, 조립
- 이순지: PPT 제작

### 개발 환경

- Windows 10
- Arduino IDE
- Kodular

### 파일 구조

- `Build/` : 빌드 파일 저장 폴더
  - `arduino_amugeona_dustsensor.apk` : 안드로이드 앱 빌드 파일
- `src/` : 소스 파일 저장 폴더
  - `app_project.aia` : Kodular (앱 인벤터) 프로젝트 파일
  - `arduino.ino` : 아두이도 소스 파일

### 시연 영상 (사진 클릭 시 유튜브로 이동합니다.)

[![시연 영상](https://img.youtube.com/vi/HoomCtO3K68/0.jpg)](https://www.youtube.com/watch?v=HoomCtO3K68 '시연 영상')

### 발표 자료

##### [클릭 시 이동합니다](https://drive.google.com/file/d/18YaGvI2Fw86gI6o8VJANANfMYeeBboxB/view?usp=sharing)

### 이 프로젝트에 사용한 것들

- LedControl 라이브러리
- SoftwareSerial 라이브러리
- EEPROM 라이브러리
- LiquidCrystal I2C 라이브러리
