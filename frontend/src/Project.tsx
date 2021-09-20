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
      title: ""
   });
   const [loading, setProjectLoading] = useState(true)

   useEffect(() => {
      getProject()
      // eslint-disable-next-line react-hooks/exhaustive-deps
   }, []);

   const getProject = async () => {
      await axios.get(process.env.REACT_APP_BACKEND + '/project/' + props.projectId)
         .then(response => setProject(response.data))
         .finally(() => setProjectLoading(false))
   }

   const onBack = () => { history.push("/"); }
   const onStructure = () => { history.push("/structure/" + props.projectId); }

   if (loading) {
      return (<><Spinner animation="border" role="status"></Spinner></>);
   }

   return (
      <>
         <Header onBack={onBack} disabledBackButton={false} title={"Project: " + project.name} />
         <Container>
            <Button onClick={onStructure}>Structure</Button>
            <MaterialList projectId={props.projectId} />
            <CrossSectionList projectId={props.projectId} />
            <LoadCaseList projectId={props.projectId} />
            <Button onClick={() => { }}>do something</Button>
         </Container>
      </>
   );
}

export default Project;
export interface RouterComponentProjectProps extends RouteComponentProps<ProjectListProps> { }
