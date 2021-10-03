import { useEffect, useState } from "react";
import { Button, Container, Spinner } from "react-bootstrap";
import { RouteComponentProps, useHistory } from "react-router-dom";
import CrossSectionList from './CrossSectionList'
import { DbProject } from './DatabaseInterface'
import MaterialList from './MaterialList'
import axios from 'axios'
import Header from "./ui/Header";
import LoadCaseList from "./LoadCaseList";

export type ProjectListProps = {
   projectId: string
}

const Project = (props: ProjectListProps) => {
   const history = useHistory();

   const [project, setProject] = useState<DbProject>({
      id: 0,
      name: "",
      title: "",
      createdAt: new Date(),
      modifiedAt: new Date()
   });

   const [calculated, setCalculated] = useState(false)
   const [loading, setProjectLoading] = useState(true)

   useEffect(() => {
      getProject()
      // eslint-disable-next-line react-hooks/exhaustive-deps
   }, []);

   const getProject = async () => {
      const response = await axios.get(process.env.REACT_APP_BACKEND + '/project/' + props.projectId)
         .finally(() => setProjectLoading(false))
      setProject(response.data)
   }

   const runCalculation = async () => {
      //! \todo hardcoded path
      const response = await axios.get(process.env.REACT_APP_SERVICE_PROVIDER + '/esbp?project=1&loadcase=1')
      setCalculated(response.data[0].status)
   }

   const openReport = async () => {
      const response = await axios.get(process.env.REACT_APP_SERVICE_PROVIDER + '/generate_report', { params: { project: props.projectId } });
      const filename = response.data[0].data;
      window.open(process.env.REACT_APP_DOWNLOAD_URL + filename, "_blank", "toolbar=yes,width=600,height=800");
   }

   const onBack = () => { history.push("/"); }
   const onStructure = () => { history.push("/structure/" + props.projectId); }

   if (loading) {
      return (<><Spinner animation="border" role="status"></Spinner></>);
   }

   return (
      <>
         <Header onBack={onBack} disabledBackButton={false} title={"Project: " + project.name} />
         <Container fluid>
            <Button onClick={onStructure}>Structure</Button>
            <MaterialList projectId={props.projectId} />
            <CrossSectionList projectId={props.projectId} />
            <LoadCaseList projectId={props.projectId} />
            <Button onClick={runCalculation}>Calculate</Button>
            <Button disabled={!calculated} onClick={openReport}>Report</Button>
         </Container>
      </>
   );
}

export default Project;
export interface RouterComponentProjectProps extends RouteComponentProps<ProjectListProps> { }
