pipeline {
    agent { label 'venus' }

    options {
        timestamps()
        timeout(time: 35, unit: 'MINUTES')
    }

    parameters {
        string(
            name: 'SDK_VERSION',
            defaultValue: '0.800.3',
            trim: true,
            description: 'Specify the Scanbot SDK version to test with'
        )
        string(
            name: 'BRANCH',
            defaultValue: 'main',
            trim: true,
            description: 'Specify the branch to checkout for testing'
        )
    }

    environment {
        BASE_IMAGE_NAME = "scanbot-sdk-base:${params.SDK_VERSION}"
        CONTAINER_WORKDIR = '/workspaces/scanbot-sdk-example-linux'
    }

    stages {

        stage('Checkout Code') {
            steps {
                checkout scm
            }
        }

        stage('Build Base Image') {
            steps {
                script {
                    withCredentials([string(credentialsId:'linux-sdk-license', variable: 'SCANBOT_SDK_LICENSE_KEY')]) {
                        // Detect the architecture of the Jenkins agent
                        def agentArch = sh(script: 'uname -m', returnStdout: true).trim()
                        def dockerArch = ''
                        
                        if (agentArch == 'x86_64') {
                            dockerArch = 'linux-x86_64'
                        } else if (agentArch == 'aarch64' || agentArch == 'arm64') {
                            dockerArch = 'linux-aarch64'
                        } else {
                            error "Unsupported agent architecture: ${agentArch}"
                        }
                        
                        echo "Detected agent architecture: ${agentArch}. Using Docker architecture: ${dockerArch}"
                        echo "Building base image with Scanbot SDK version: ${params.SDK_VERSION}"
                         
                        sh """
                            docker build \\
                                --build-arg SDK_VERSION='${params.SDK_VERSION}' \\
                                --build-arg ARCH='${dockerArch}' \\
                                --build-arg SCANBOT_LICENSE='${SCANBOT_SDK_LICENSE_KEY}' \\
                                --target sdk-verification \\
                                -t ${env.BASE_IMAGE_NAME} \\
                                .
                        """
                    }
                }
            }
        }

        stage('Run Tests in Parallel') {
            parallel {
                stage('Python Tests') {
                    steps {
                        echo "Running Python tests..."
                        sh """
                            docker run --rm \\
                                --env CI_MODE=true \\
                                --workdir ${env.CONTAINER_WORKDIR} \\
                                ${env.BASE_IMAGE_NAME} \\
                                /tests/test-python.sh
                        """
                    }
                }
                
                stage('Java Tests') {
                    steps {
                        echo "Running Java tests..."
                        sh """
                            docker run --rm \\
                                --env CI_MODE=true \\
                                --workdir ${env.CONTAINER_WORKDIR} \\
                                ${env.BASE_IMAGE_NAME} \\
                                /tests/test-java.sh
                        """
                    }
                }
                
                stage('Node.js Tests') {
                    steps {
                        echo "Running Node.js tests..."
                        sh """
                            docker run --rm \\
                                --env CI_MODE=true \\
                                --workdir ${env.CONTAINER_WORKDIR} \\
                                ${env.BASE_IMAGE_NAME} \\
                                /tests/test-nodejs.sh
                        """
                    }
                }
                
                stage('C Tests') {
                    steps {
                        echo "Running C tests..."
                        sh """
                            docker run --rm \\
                                --env CI_MODE=true \\
                                --workdir ${env.CONTAINER_WORKDIR} \\
                                ${env.BASE_IMAGE_NAME} \\
                                /tests/test-c.sh
                        """
                    }
                }
            }
        }
    }

    post {
        success {
            echo 'All platform tests PASSED!'
            echo 'All 4 platforms validated successfully'
            echo 'Code is ready for merge'
        }
        failure {
            echo 'Some platform tests FAILED!'
            echo 'Check individual stage logs above'
            echo 'Fix failing platforms before merge'
        }
        always {            
            sh """
                echo "Cleaning up Docker resources..."
                docker rmi ${env.BASE_IMAGE_NAME} || true
                docker system prune -f --filter "until=2h" || true
                echo "Cleanup complete"
            """
            
            echo "Job finished. Cleaning up workspace."
            cleanWs()
        }
    }
}
