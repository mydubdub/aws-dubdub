# The AWS x Dub-Dub Thesis Project
by T.C. Tantokusumo BE-2018 and A.A. Iskandar
Swiss German University

## 1. Objective

The main objective of this project is to provide a telemedicine solution for remote area that has a prevalence of high heart diseases by performing a pre-screening using Dub-Dub.

## 2. Previous Studies
### A. DEVELOPMENT OF A WEARABLE ECG DEVICE USING LOW POWER CONSUMPTION FOR DETECTING ARRYTHMIA DISEASES (Hanali, 2019)


### B. REAL-TIME WEARABLE ECG SIGNAL ANALYSIS FOR CARDIAC ARRHYTHMIA DETECTION (Sugiarto, 2019)
![image](https://user-images.githubusercontent.com/61266571/171039054-2d310b3b-5b36-4772-9701-87a4916c3934.png)
Flowchart of ECG feature extraction software connected to smartphone display via bluetooth connection (Sugiarto, 2019)

Sugiarto and Iskandar's algorithm has been implemented into the wearable ECG device through smartphone Bluetooth connection to extract essential ECG characteristics. This extraction aids to classify a sort of peaks for cardiac arrhythmia detection. A combination of Pan-Tompkins Algorithm and Kim-Lee formula was used in order to extract the ECG characteristics (Kim et al., 2015). The average segment characterization accuracy was about 90%, with the fault being mostly owing to the failure to recognize R-peak. AF, SVT, and PVC are all correctly predicted by the system. 

### C. DEVELOPMENT OF CLOUD COMPUTING ALGORITHM FOR ARRHYTHMIA DETECTION AND HEART DISEASES (Setiawan, 2020)
![image](https://user-images.githubusercontent.com/61266571/171039140-45440d5d-05c5-49f5-aa64-aa5e59097377.png)
Web Application Data Flow (Setiawan, 2020)

In Setiawan’s work, a web application is developed to examine ECG signal and heart condition of patient that can be accessed by users such as cardiologists or patient itself. Sugiarto’s ECG extraction parameter code was adopted to analyse the important feature of the ECG signal. This web application was able to run in localhost and Node JS as the runtime-environment. Thus, in this work, the web application deployment is only available on premise, i.e., can only be access locally. However, Setiawan suggests that the work can be further implemented to a dedicated server so that it can be publicized.

## 3. Propose System Design
![VirtualAcessDesign drawio (2)](https://user-images.githubusercontent.com/61266571/170742649-e8cb3709-263a-4fbc-933a-32d484e09800.png)
Figure of Propose System Design

The purpose of this research is to develop a virtual access system on cloud environment which can be accessed by users through any internet-based platform, including smartphone or computer. The developed cloud system can handle ECG data transmission from a wearable ECG device worn by patients at a rural public health center to a cloud storage or database for storing the recorded data. Afterwards, the recorded data is conveyed to the previously developed web application in virtual private server to extract important parameters to detect cardiac arrhythmias. In this stage, the processes are done completely on cloud environment. Next, a cardiologist at a district hospital accesses the web application through virtual private server to obtain the computed data. As a result, the cardiologist can provide virtual diagnosis back to the patient at rural public health center based on the computed data.

## 4. Final System Architecture 
![Virtual System DubDub_version 2 drawio](https://user-images.githubusercontent.com/61266571/170742220-c34fe6a4-7570-40c3-866e-70bf0ee54e3b.png)

The final architecture system design consists of two main parts, data transmission & storing architecture and the virtual access system. For the data transmission & storing, it consists of DubDub Portable ECG Device as ECG signal recorder and AWS IOT Core to connect the device with the AWS cloud environment. Then, the recorded ECG signal data is directed using a delivery system called AWS Kinesis Data Firehose to a static content storage, AWS S3.

In the virtual access system, the AWS EC2 hosts the previously developed web application utilizing Node JS as the runtime-environment. This web application is able to retrieve previously stored data in AWS S3 and download it to a local computer. In addition, a compiled C++ language program is integrated to the web application to extract the ECG signal feature. To begin the extraction, the previously downloaded ECG signal data is upload back to the web application and directly computed by the compiled program. After the computation, the result files are then stored back to AWS S3. In order to show the computation result, the computed files are called back to the application and display on the web application. As a result, the users such as cardiologists or patients can examine the computed ECG signal data.

### 3.2. AWS Configuration

### 3.3. AWS IoT Core

### 3.4. Kinesis Data Firehose

### 3.5. Simple Storage Service (S3)

### 3.6. Elastic Cloud Computing (EC2)




## Welcome to GitHub Pages

You can use the [editor on GitHub](https://github.com/mydubdub/aws-dubdub/edit/gh-pages/index.md) to maintain and preview the content for your website in Markdown files.

Whenever you commit to this repository, GitHub Pages will run [Jekyll](https://jekyllrb.com/) to rebuild the pages in your site, from the content in your Markdown files.

### Markdown

Markdown is a lightweight and easy-to-use syntax for styling your writing. It includes conventions for

```markdown
Syntax highlighted code block

# Header 1
## Header 2
### Header 3

- Bulleted
- List

1. Numbered
2. List

**Bold** and _Italic_ and `Code` text

[Link](url) and ![Image](src)
```

For more details see [Basic writing and formatting syntax](https://docs.github.com/en/github/writing-on-github/getting-started-with-writing-and-formatting-on-github/basic-writing-and-formatting-syntax).

### Jekyll Themes

Your Pages site will use the layout and styles from the Jekyll theme you have selected in your [repository settings](https://github.com/mydubdub/aws-dubdub/settings/pages). The name of this theme is saved in the Jekyll `_config.yml` configuration file.

### Support or Contact

Having trouble with Pages? Check out our [documentation](https://docs.github.com/categories/github-pages-basics/) or [contact support](https://support.github.com/contact) and we’ll help you sort it out.
