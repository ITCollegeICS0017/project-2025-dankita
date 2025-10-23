Photo Studio Management System

- Team members (names + GitHub logins)

  - Cebanu Dan — @Nik0gda
  - Nikita Springis — @nikitaspringis21
  - Manana Bebia — @Mabebi29
  - Anna Sukhanishvili — @annasulkh

- Short description

  - The system supports a photo studio workflow where a receptionist records client orders (photo printing, film developing) with client surname and desired completion time. Orders are issued on two forms (client and photographer). Pricing depends on urgency, with express orders incurring a 25% surcharge; payment is collected upon completion. Throughout the day the receptionist forwards orders to the photographer, who processes them using consumables tracked by the administrator. End-of-day reports include completed orders and revenue from the receptionist, and consumed materials from the photographer.

- Build/run instructions
  - Prerequisites: Make and a C++ toolchain available in your environment.
  - Build and run:
    - `make -jN` N - number of threads used to compile
  - Run tests:
    - `make test`

## Release workflow

- We keep ONE repository for the whole project.
- At the end of each checkpoint, mark a release tag and add your report.

### Mark a release tag

### end of Release 1:

git tag release-1
git push origin release-1

### end of Release 2:

git tag release-2
git push origin release-2

### Folders for documents

docs/release-1/ # slides, SRS/SDP updates, test report

docs/release-2/

docs/release-3/
