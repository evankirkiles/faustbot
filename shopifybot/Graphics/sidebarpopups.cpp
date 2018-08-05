//
// Created by Evan Kirkiles on 8/1/18.
//

#include "sidebarpopups.hpp"

// PROFILES DISPLAY
// Constructor that builds the profiles display
ProfilesDisplay::ProfilesDisplay(QWidget *parent) : addProfileButton(new ClickableImage(26, 26, 2, file_paths::ADD2_IMG,
                                                                                        file_paths::ADD_IMG, this)),
                                                    duplicateProfileButton(new ClickableImage(26, 26, 2, file_paths::DUPLICATE2_IMG,
                                                                                        file_paths::DUPLICATE_IMG, this)),
                                                    deleteProfileButton(new ClickableImage(26, 26, 2, file_paths::MINUS2_IMG,
                                                                                        file_paths::MINUS_IMG, this)),
                                                    profilesListView(new QListWidget(this)),
                                                    titleLabel(new QLabel("Title: ", this)),
                                                    editTitle(new QLineEdit(this)),
                                                    emailLabel(new QLabel("Email:", this)),
                                                    email(new QLineEdit(this)),
                                                    firstnameLabel(new QLabel("First name:", this)),
                                                    firstname(new QLineEdit(this)),
                                                    lastnameLabel(new QLabel("Last name:", this)),
                                                    lastname(new QLineEdit(this)),
                                                    address1Label(new QLabel("Address:", this)),
                                                    address1(new QLineEdit(this)),
                                                    address2Label(new QLabel("Apartment, Suite, etc:", this)),
                                                    address2(new QLineEdit(this)),
                                                    cityLabel(new QLabel("City:", this)),
                                                    city(new QLineEdit(this)),
                                                    zipcodeLabel(new QLabel("Zip Code:", this)),
                                                    zipcode(new QLineEdit(this)),
                                                    provinceLabel(new QLabel("Province:", this)),
                                                    province(new QLineEdit(this)),
                                                    countryLabel(new QLabel("Country:", this)),
                                                    country(new QLineEdit(this)),
                                                    phoneLabel(new QLabel("Phone:", this)),
                                                    phone(new QLineEdit(this)),
                                                    ccardLabel(new QLabel("Credit Card:", this)),
                                                    ccard(new QComboBox(this)),
                                                    update(new QPushButton("UPDATE", this)),
                                                    QWidget(parent) {

    // Set window properties
    setFixedSize(500, 500);
    setWindowTitle("Profiles");
    setWindowFlags(Qt::FramelessWindowHint);
    setFocusPolicy(Qt::ClickFocus);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_TranslucentBackground);

    // Create the dark title bar
    dtb = new DarkTitleBar(this, true);

    // Set the stylesheet for the window
    QFile File("./shopifybot/Graphics/stylesheet.qss");
    File.open(QFile::ReadOnly);
    QString StyleSheet = QLatin1String(File.readAll());
    setStyleSheet(StyleSheet);

    // Create external qframe and layouts for dtb
    auto externLayout = new QVBoxLayout();
    externLayout->setContentsMargins(0, 0, 0, 0);
    auto bg = new QFrame(this);
    auto bgLayout = new QVBoxLayout();
    bgLayout->setContentsMargins(0, 0, 0, 0);
    bg->setObjectName("main_window");
    bg->setLayout(bgLayout);
    bgLayout->addWidget(dtb);
    externLayout->addWidget(bg);

    // Create normal layouts
    auto mainLayout = new QHBoxLayout();
    mainLayout->setContentsMargins(11, 3, 11, 11);
    QSizePolicy spLeft(QSizePolicy::Preferred, QSizePolicy::Preferred);
    spLeft.setHorizontalStretch(1);
    auto leftColumnWidg = new QWidget();
    auto leftColumn = new QVBoxLayout();
    leftColumnWidg->setLayout(leftColumn);
    leftColumnWidg->setSizePolicy(spLeft);
    leftColumn->setContentsMargins(0, 0, 0, 0);
    auto smallButtonRow = new QHBoxLayout();
    QSizePolicy spRight(QSizePolicy::Preferred, QSizePolicy::Preferred);
    spRight.setHorizontalStretch(2);
    smallButtonRow->addWidget(addProfileButton);
    smallButtonRow->addWidget(duplicateProfileButton);
    smallButtonRow->addWidget(deleteProfileButton);
    auto rightColumnWidg = new QWidget();
    auto rightColumn = new QVBoxLayout();
    rightColumnWidg->setLayout(rightColumn);
    rightColumnWidg->setSizePolicy(spRight);
    rightColumn->setContentsMargins(0, 0, 0, 0);
    auto emailRow = new QHBoxLayout();
    auto titleRow = new QHBoxLayout();
    auto fNameRow = new QHBoxLayout();
    auto lNameRow = new QHBoxLayout();
    auto address1Row = new QHBoxLayout();
    auto address2Row = new QHBoxLayout();
    auto cityZipRow = new QHBoxLayout();
    auto countryRow = new QHBoxLayout();
    auto phoneRow = new QHBoxLayout();
    auto ccardRow = new QHBoxLayout();

    // Create the widgets and add them to their respective layouts
    leftColumn->addLayout(smallButtonRow);
    profilesListView->setObjectName("profileslistview");
    profilesListView->setSelectionMode(QAbstractItemView::SingleSelection);
    leftColumn->addWidget(profilesListView);
    titleLabel->setObjectName("task_important_text");
    editTitle->setObjectName("task_title_lineedit");
    titleRow->addWidget(titleLabel);
    titleRow->addWidget(editTitle);
    emailLabel->setObjectName("addtask_mediocre_text");
    email->setObjectName("addtask_editbox");
    emailRow->addWidget(emailLabel);
    emailRow->addWidget(email);
    firstnameLabel->setObjectName("addtask_mediocre_text");
    firstname->setObjectName("addtask_editbox");
    fNameRow->addWidget(firstnameLabel);
    fNameRow->addWidget(firstname);
    lastnameLabel->setObjectName("addtask_mediocre_text");
    lastname->setObjectName("addtask_editbox");
    lNameRow->addWidget(lastnameLabel);
    lNameRow->addWidget(lastname);
    address1Label->setObjectName("addtask_mediocre_text");
    address1->setObjectName("addtask_editbox");
    address1Row->addWidget(address1Label);
    address1Row->addWidget(address1);
    address2Label->setObjectName("addtask_mediocre_text");
    address2->setObjectName("addtask_editbox");
    address2Row->addWidget(address2Label);
    address2Row->addWidget(address2);
    cityLabel->setObjectName("addtask_mediocre_text");
    city->setObjectName("addtask_editbox");
    cityZipRow->addWidget(cityLabel);
    cityZipRow->addWidget(city);
    zipcodeLabel->setObjectName("addtask_mediocre_text");
    zipcode->setObjectName("addtask_editbox");
    zipcode->setMaximumWidth(60);
    cityZipRow->addWidget(zipcodeLabel);
    cityZipRow->addWidget(zipcode);
    countryLabel->setObjectName("addtask_mediocre_text");
    country->setObjectName("addtask_editbox");
    provinceLabel->setObjectName("addtask_mediocre_text");
    province->setObjectName("addtask_editbox");
    countryRow->addWidget(countryLabel);
    countryRow->addWidget(country);
    countryRow->addWidget(provinceLabel);
    countryRow->addWidget(province);
    phoneLabel->setObjectName("addtask_mediocre_text");
    phone->setObjectName("addtask_editbox");
    phoneRow->addWidget(phoneLabel);
    phoneRow->addWidget(phone);
    ccardLabel->setObjectName("addtask_mediocre_text");
    update->setObjectName("addtaskbutton");
    update->setFixedSize(100, 35);
    ccardRow->addWidget(ccardLabel);
    ccardRow->addWidget(ccard);
    ccardRow->addWidget(update);

    // Finish adding all the layouts
    mainLayout->addWidget(leftColumnWidg);
    mainLayout->addWidget(rightColumnWidg);
    rightColumn->addLayout(titleRow);
    rightColumn->addLayout(emailRow);
    rightColumn->addLayout(fNameRow);
    rightColumn->addLayout(lNameRow);
    rightColumn->addLayout(address1Row);
    rightColumn->addLayout(address2Row);
    rightColumn->addLayout(cityZipRow);
    rightColumn->addLayout(countryRow);
    rightColumn->addLayout(phoneRow);
    rightColumn->addLayout(ccardRow);
    bgLayout->addLayout(mainLayout);

    // Set the layout finally
    setLayout(externLayout);

    // Connect the update button to the submit
    connect(update, SIGNAL(clicked()), this, SLOT(submit()));
    // Connect the select function to the signal produced by the qlistwidget
    connect(profilesListView, SIGNAL(currentTextChanged(QString)), this, SLOT(select(QString)));
    // Connect the add new function to the signal produced by clicking the plus button
    connect(addProfileButton, SIGNAL(clicked()), this, SLOT(createNew()));

    // Fill in the profiles listview
    refresh();
}
// Override the column for the profiles display
void ProfilesDisplay::closeEvent(QCloseEvent* event) {
    emit closed();
    QWidget::closeEvent(event);
}
// Refreshes the list of profiles, should be called after updates
void ProfilesDisplay::refresh(QModelIndex selected) {
    // Rereads the profiles file and builds the qlist of profile titles for the listview
    std::ifstream fin(file_paths::PROFILES_TXT);
    std::string str;

    // Reset the list view
    profilesListView->clear();

    // Cycle through the lines and get the title of each profile (comes before the ":-:")
    while (getline(fin, str)) {
        // Check for the title identifier and add the preceding substring to the qlist
        const unsigned long titlePos = str.find(" :-:");
        if (titlePos != std::string::npos) {
            profilesListView->addItem(str.substr(0, titlePos).c_str());
        }
    }

    // Close the file
    fin.close();

    // Select a specified item, useful for the submit function
    if (selected.isValid()) {
        profilesListView->setCurrentIndex(selected);
    } else {
        profilesListView->setCurrentIndex(profilesListView->indexAt(QPoint(profilesListView->count() - 1,0)));
    }
}
// Changes the text fields to the selected profile's information
void ProfilesDisplay::select(QString which) {
    // Reads the profiles file and finds the JSON data for the given profile title
    std::ifstream fin(file_paths::PROFILES_TXT);
    std::string str;

    // Cycle through the lines and get the title of each profile
    while (getline(fin, str)) {
        // Check for the title identifier and parse through its JSON data
        const unsigned long titlePos = str.find(which.toStdString());
        if (titlePos != std::string::npos) {
            str = str.substr(titlePos + which.toStdString().size() + 5);
            QJsonDocument jsonDoc = QJsonDocument::fromJson(QString(str.c_str()).toUtf8());
            QJsonObject jsonObject = jsonDoc.object();

            // Use the JSON object to fill in the data fields
            editTitle->setText(which);
            editTitle->setStyleSheet("color: #baa86a;");
            email->setText(jsonObject.value("email").toString());
            email->setStyleSheet("color: #baa86a;");
            firstname->setText(jsonObject.value("firstname").toString());
            firstname->setStyleSheet("color: #baa86a;");
            lastname->setText(jsonObject.value("lastname").toString());
            lastname->setStyleSheet("color: #baa86a;");
            address1->setText(jsonObject.value("address1").toString());
            address1->setStyleSheet("color: #baa86a;");
            address2->setText(jsonObject.value("address2").toString());
            address2->setStyleSheet("color: #baa86a;");
            city->setText(jsonObject.value("city").toString());
            city->setStyleSheet("color: #baa86a;");
            country->setText(jsonObject.value("country").toString());
            country->setStyleSheet("color: #baa86a;");
            province->setText(jsonObject.value("province").toString());
            province->setStyleSheet("color: #baa86a;");
            zipcode->setText(jsonObject.value("zipcode").toString());
            zipcode->setStyleSheet("color: #baa86a;");
            phone->setText(jsonObject.value("phone").toString());
            phone->setStyleSheet("color: #baa86a;");
            ccard->setCurrentText(jsonObject.value("ccard").toString());
            ccard->setStyleSheet("color: #baa86a;");

            break;
        }
    }
}
// Submits the text to the profiles.txt
void ProfilesDisplay::submit() {
    // Initialize input/output streams
    std::ifstream filein(file_paths::PROFILES_TXT);
    std::ofstream fileout(file_paths::TEMPPROFILES_TXT);
    if (!filein || !fileout) { throw std::runtime_error("Error opening profiles file."); }

    // Cycle through the input file until the correct line is found
    std::string strTemp;
    bool titleNotChanged = true;
    while (getline(filein, strTemp)) {
        // When it finds the correct title, change the text to the JSON formatted string
        if (strTemp.substr(0, strTemp.find(" :-:")) == profilesListView->currentItem()->text().toStdString() && titleNotChanged) {

            // If there already exists a profile by the same name, edit the title

            strTemp = getSafeName(profilesListView->currentRow(), editTitle->text().toStdString()) +
                    R"( :-: {"email":")" + email->text().toStdString() +
                    R"(","firstname":")" + firstname->text().toStdString() + R"(","lastname":")" +
                      lastname->text().toStdString() + R"(","address1":")" + address1->text().toStdString() +
                    R"(","address2":")" + address2->text().toStdString() + R"(","city":")" +
                      city->text().toStdString() + R"(","country":")" + country->text().toStdString() +
                      R"(","province":")" + province->text().toStdString() + R"(","zipcode":")" +
                      zipcode->text().toStdString() + R"(","phone":")" + phone->text().toStdString() +
                      R"(","ccard":")" + ccard->currentText().toStdString() + R"("})";
            titleNotChanged = false;
        }

        // Finalize the string formatting and write it to the output file
        fileout << strTemp << "\n";
    }

    // Close the files used
    filein.close();
    fileout.close();

    // Rename the output file to the input file so it overwrites it and is picked up by the listview
    std::rename(file_paths::TEMPPROFILES_TXT, file_paths::PROFILES_TXT);

    // Finally update the qlistview display
    refresh(profilesListView->currentIndex());
}

// Slot that creates a new profile (called when the plus button is clicked)
void ProfilesDisplay::createNew() {

    // Opens the profiles file and adds a new profile to the end
    std::ofstream profilesFile(file_paths::PROFILES_TXT, std::ios_base::app | std::ios_base::out);
    profilesFile << getSafeName() + R"( :-: {"email":","firstname":"","lastname":"","address1":"","address2":"","city":"","country":"","province":"","zipcode":"","phone":"","ccard":"Random"})" + "\n";
    profilesFile.close();

    // Then refreshes the profiles view and selects the most recent addition
    refresh();
}

// Gets a safe, unique name for a profile, default is Untitled (for add new case)
std::string ProfilesDisplay::getSafeName(const int currentIndex, std::string title) {

    // Remove whitespace from the end of the title
    rtrim(title);

    // Iterate through the names and search for number add-on availiability
    bool nameFound = false;
    unsigned int modNum = 0;
    std::string modifier;
    while (!nameFound) {

        // Iterate through the current profile names
        for (int i = 0; i < profilesListView->count(); i++) {
            nameFound = false;
            QListWidgetItem* item = profilesListView->item(i);
            // Skip the current index of the profile being edited
            if (i == currentIndex) { nameFound = true; continue; }

            // If a match is found, update the modifier and rerun the loop
            if (item->text().toStdString() == title + modifier) {
                modNum++;
                modifier = " (" + std::to_string(modNum) + ")";
                break;
            }
            nameFound = true;
        }
    }

    return title + modifier;
}